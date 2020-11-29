<?php
/**
 * Created by PhpStorm.
 * User: MegaVolt
 * Date: 24.11.2020
 * Time: 14:03
 */

class Orders
{
    /**
     * Массив доступных заказов. Будут отфильтрованы либо по коду юзера, либо если админ - то все
     *
     * @var array
     */
    private $orders = [];

    /**
     * @return array
     */
    public function getOrders(): array
    {
        return $this->orders;
    }

    /**
     * @var DB $instance
     */
    private static $instance = null;

    static function getInstance()
    {
        if (self::$instance === null) {
            self::$instance = new Orders();
        }
        return self::$instance;
    }

    private function __construct()
    {
        if (!isUser() && !isAdmin()) throw new Exception("Для работы с заказами требуется авторизация");
        $this->reReadOrders();
    }

    private function __clone()
    {
    }

    private function reReadOrders()
    {
        if (isUser()) {
            $this->orders = DB::getInstance()->QueryMany("SELECT o.*, os.name as status_name, os.description from orders o 
                        inner join order_status os on o.status_id=os.id
                        where o.user_id=?", $_SESSION['user_id']);
            return;
        }

        if (isAdmin()) {
            $this->orders = DB::getInstance()->QueryMany("SELECT o.*, os.name as status_name, u.login as user_login from orders o 
                                                                inner join order_status os on o.status_id=os.id
                                                                inner join users u on u.id=o.user_id");
        }
    }

    static function ajaxCreateFromCart()
    {
        $response['result'] = 1;
        try {
            if (!isUser()) throw new Exception("Только для авторизованных пользователей!");

            Orders::getInstance()->createFromCart();
            http_response(200, json_encode($response), 'application/json');
        } catch (Exception $e) {
            $response['result'] = 0;
            $response['errorMessage'] = $e->getMessage();
            http_response(400, json_encode($response), 'application/json');
        }
    }

    /**
     * Создание заказа пользователем
     *
     * @throws Exception
     */
    private function createFromCart()
    {
        // Корзина текущего ползьвателя со скидками
        // и одновременным отфильтровыванием отсутствующих товаров
        $goodsInCart = Cart::getInstance()->getGoodsInCartApplyDiscount(new Discount());

        if (!$goodsInCart) throw new Exception('Корзина пуста!');

        // сумма всего заказа
        $total_vsego = 0.0;
        foreach ($goodsInCart as $good) {
            $total_vsego += $good['vsego'];
        }

        // запись заказа
        DB::getInstance()->StartTransaction();
        $this->defaultOrder($order);
        $order['vsego'] = $total_vsego;
        $order['user_id'] = $_SESSION['user_id'];
        $order['order_date'] = date("Y-m-d H:i:s");
        $order['status_id'] = 1; // Новый заказ
//        DB::getInstance()->QueryOne("INSERT INTO orders set vsego=?, order_date=now(), user_id=?, status_id=1", $total_vsego, $_SESSION['user_id']);
        $this->save($order, $errors);
        $order_id = $order['id'];

        // подробности заказа
        $values = [];
        foreach ($goodsInCart as $good) {
            $values[] = DB::getInstance()->PrepareStatement("(?,?,?,?,?,?,?,?)"
                , $order_id, $good['goods_id']
                , $good['quantity'], $good['price']
                , $good['discount'], $good['itogo']
                , $good['vsego'], $good['discountMessage']);
        }
        DB::getInstance()->QueryOne("INSERT INTO orders_detail (order_id, goods_id, quantity, price, discount, itogo, vsego, discount_message) values " . implode(',', $values));

        // очистим корзину
        DB::getInstance()->QueryOne("DELETE FROM cart WHERE user_id=?", $_SESSION['user_id']);

        DB::getInstance()->CommitTransaction();

        $this->reReadOrders();
    }

    // ajaxGetOrder
    static function ajaxGetOrder($order_id)
    {
        $response['result'] = 1;
        try {
            if (!isUser() && !isAdmin()) throw new Exception("Только для авторизованных пользователей!");

            $response['data'] = Orders::getInstance()->getById($order_id);
            $response['data']['detail'] = Orders::getInstance()->getOrderDetail($order_id);

            http_response(200, json_encode($response), 'application/json');
        } catch (Exception $e) {
            $response['result'] = 0;
            $response['errorMessage'] = $e->getMessage();
            http_response(400, json_encode($response), 'application/json');
        }
    }

    /**
     * Получим детализацию заказа
     *
     * @param $order_id
     * @return array
     * @throws Exception
     */
    public function getOrderDetail($order_id)
    {
        return DB::getInstance()->QueryMany("SELECT od.*, g.name FROM orders_detail od
                                                      left join goods g on g.id=od.goods_id where order_id=?", $order_id);
    }

    static public function ajaxSaveUpdateOrder($order_id)
    {
        $response['result'] = 1;
        try {
            if (!isUser() && !isAdmin()) throw new Exception("Только для авторизованных пользователей!");

            $json_data = file_get_contents("php://input");
            $order = json_decode($json_data, true);
            $order['id'] = $order_id;

            $errors = [];
            Orders::getInstance()->save($order, $errors);
            if ($errors) throw new Exception(implode('<br>', $errors));

            http_response(200, json_encode($response), 'application/json');
        } catch (Exception $e) {
            $response['result'] = 0;
            $response['errorMessage'] = $e->getMessage();
            http_response(400, json_encode($response), 'application/json');
        }
    }

    /**
     * Сохраняет массив в базу данных. Обновляет поля существующей или добавляет новую запись.
     *
     * @param array $order
     * @param array $errors
     * @throws Exception
     */
    public function save(&$order, &$errors = [])
    {
        // надо сохранить ид. даже пустой.
        $id = @$order['id'];

        // отфильтруем отсутствующие в БД поля и восстановим регистр названий полей
        $this->_restoreFieldNames($order);
        $order['id'] = $id;

        // проверка валидности данных
        $errors = $this->_checkParameters($order);
        if ($errors) return;

        if (!$id) {
            // новый заказ
            self::_saveInsert($order);
        } else {
            // изменение заказа
            self::_saveUpdate($order);
        }
    }

    /**
     * Обновляет поля существующего заказа
     * @param $order
     * @throws Exception
     */
    private function _saveUpdate($order)
    {
        $dbt = $this->getById($order['id']);
        if (!$dbt) {
            throw new Exception('Заказ с кодом ' . $order['id'] . 'либо не существует, либо нет прав доступа.');
        }

        $order = array_merge($dbt, $order);

        $fields = [];
        $params = [];
        foreach ($dbt as $key => $value) {
            if (strcmp($value, $order[$key])) {
                $fields[] = $key . '=?';
                $params[] = $order[$key];
            }
        }

        if (!$fields) return; // нет изменений

        $sql = "UPDATE orders SET " . implode(', ', $fields);
        $sql .= " WHERE id=? LIMIT 1;";
        array_unshift($params, $sql);
        array_push($params, $order['id']);
        DB::getInstance()->QueryOne(...$params);
    }

    /**
     * Создание нового заказа. По идее, прийти могут не все поля. Надо отфильтровать те, которых нет и сохранить остальные.
     *
     * @param $order
     * @throws Exception
     */
    private function _saveInsert(&$order)
    {
        $defaultOrder = [];
        $this->defaultOrder($defaultOrder);
        $order = array_merge($defaultOrder, $order);

        $fields = [];
        $values = [];
        foreach ($defaultOrder as $key => $value) {
            $fields[] = $key . '=?';
            $values[] = $order[$key];
        }
        $sql = "INSERT INTO orders SET " . implode(', ', $fields);
        array_unshift($values, $sql);
        DB::getInstance()->QueryOne(...$values);

        $order['id'] = DB::getInstance()->LastInsertId();
        if (empty($order['id'])) throw new Exception("Ошибка при создании заказа");
    }

    /**
     * Проверяет поля на допустимые значения
     *
     * @param array $order
     * @return array Массив с ошибками
     * @throws Exception
     */
    private function _checkParameters($order)
    {
        $errors = [];
        // тут можно побездельничать. При неверных ид выругается сама БД, т.к. будет нарушаться ее целостность и консистентность
        return $errors;
    }

    /**
     * Функция корректирет имена полей в запросе. Делает так, как в базе данных
     *
     * @param $order
     * @throws Exception
     */
    private function _restoreFieldNames(&$order)
    {
        $data = [];
        $defaultOrder = [];
        $this->defaultOrder($defaultOrder);
        foreach ($defaultOrder as $key => $value) {
            $lowerKey = mb_strtolower($key);
            if (isset($order[$key])) {
                $data[$key] = $order[$key];
            } elseif (isset($order[$lowerKey])) {
                $data[$key] = $order[$lowerKey];
            }
        }
        $order = $data;
    }

    /**
     * Создается пустой массив, содержащий поля из таблицы заказа
     *
     * @param $order
     * @throws Exception
     */
    public function defaultOrder(&$order)
    {
        $order = [];
        // получим поля
        $describe_order = DB::getInstance()->QueryMany("DESCRIBE orders;");
        foreach ($describe_order as $row) {
            if ($row['Field'] === 'id') continue;
            $order[$row['Field']] = (($row['Default'] != '') ? $row['Default'] : '');
        }
    }

    /**
     * Получить заказ в виде массива по коду id
     * @param $id
     * @return array|bool
     * @throws Exception
     */
    public function getById($id)
    {
        $orders = array_filter($this->getOrders(), function ($order) use ($id) {
            return $order['id'] == $id;
        });
        if ($orders) return current($orders);
        return [];
    }
}
