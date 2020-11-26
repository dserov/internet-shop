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
                        inner join order_status os on o.status_id=os.id where o.user_id=?", $_SESSION['user_id']);
            return;
        }

        if (isAdmin()) {
            $this->orders = DB::getInstance()->QueryMany("SELECT o.*, os.name as status_name from orders o inner join order_status os on o.status_id=os.id");
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
        DB::getInstance()->QueryOne("INSERT INTO orders set vsego=?, order_date=now(), user_id=?, status_id=1", $total_vsego, $_SESSION['user_id']);
        $order_id = DB::getInstance()->LastInsertId();

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
            if (!isUser()) throw new Exception("Только для авторизованных пользователей!");

            $response['data'] = Orders::getInstance()->getOrder($order_id);

            http_response(200, json_encode($response), 'application/json');
        } catch (Exception $e) {
            $response['result'] = 0;
            $response['errorMessage'] = $e->getMessage();
            http_response(400, json_encode($response), 'application/json');
        }
    }

    /**
     * Получим заказ вместе с детализацией
     *
     * @param $order_id
     * @return array
     * @throws Exception
     */
    public function getOrder($order_id) {
        $order = array_filter($this->getOrders(), function ($item) use ($order_id) {
            return $item['id'] == $order_id;
        });

        if (!$order) {
            return [];
        }
        $order = current($order);

        // детали заказа
        $order['detail'] = DB::getInstance()->QueryMany("SELECT od.*, g.name FROM orders_detail od
                                                      left join goods g on g.id=od.goods_id where order_id=?", $order_id);
        return $order;
    }
}
