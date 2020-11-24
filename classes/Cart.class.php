<?php


class Cart
{
    /**
     * @var array
     */
    private $goodsInCart = [];

    /**
     * @return array
     */
    function getGoodsInCart(): array
    {
        return $this->goodsInCart;
    }

    /**
     * @var DB $instance
     */
    private static $instance = null;

    static function getInstance()
    {
        if (self::$instance === null) {
            self::$instance = new Cart();
        }
        return self::$instance;
    }

    private function __construct()
    {
        if (!isUser()) return;
        $this->reReadCart();
    }

    private function __clone()
    {
    }

    private function reReadCart()
    {
        $this->goodsInCart = DB::getInstance()->QueryMany("SELECT g.id AS goods_id, g.name, g.price, c.quantity, c.user_id 
        FROM cart c INNER JOIN goods g ON c.goods_id = g.id where c.user_id=?", $_SESSION['user_id']);
    }

    /**
     * @param $data
     * @throws Exception
     */
    private function addGoods($data)
    {
        // проверка входных данных
        $product = DB::getInstance()->QueryOne("select * from goods where id=?", $data['id_product']);
        if (!$product) throw new Exception('Такой продукт не найден');
        if (!is_numeric($data['quantity']) || $data['quantity'] <= 0)
            throw new Exception('Количество указано неверно');

        // проверка наличия товара в корзине
        $goodInCart = array_filter($this->goodsInCart, function ($item) {
            return $item['user_id'] == $_SESSION['user_id'] && $item['goods_id'] == 2;
        });
        $goodInCart = current($goodInCart);
        if ($goodInCart) {
            // установка количества, либо суммирование количества с тем, что уже в корзине
            if (!isset($data['action']) || $data['action'] != 'set') {
                $data['quantity'] += $goodInCart['quantity'];
            }
            DB::getInstance()->QueryOne("UPDATE cart SET quantity=? where goods_id=? and user_id=?",
                $data['quantity'], $product['id'], $_SESSION['user_id']);
        } else {
            DB::getInstance()->QueryOne("INSERT INTO cart (goods_id, user_id, quantity) values (?,?,?)",
                $product['id'], $_SESSION['user_id'], $data['quantity']);
        }

        $this->reReadCart();
    }

    static function ajaxAddGoods()
    {
        $response['result'] = 1;
        try {
            if (!isUser()) throw new Exception("Только для авторизованных пользователей!");

            $json_data = file_get_contents("php://input");
            $data = json_decode($json_data, true);

            $cart = Cart::getInstance();
            $cart->addGoods($data);

            http_response(200, json_encode($response), 'application/json');
        } catch (Exception $e) {
            $response['result'] = 0;
            $response['errorMessage'] = $e->getMessage();
            http_response(400, json_encode($response), 'application/json');
        }
    }

    /**
     * @param $data
     * @throws Exception
     */
    private function removeGoods($data)
    {
        DB::getInstance()->QueryOne("DELETE FROM cart WHERE goods_id=? and user_id=?", $data['id_product'], $_SESSION['user_id']);
        $this->reReadCart();
    }

    static function ajaxRemoveGoods()
    {
        $response['result'] = 1;
        try {
            if (!isUser()) throw new Exception("Только для авторизованных пользователей!");

            $json_data = file_get_contents("php://input");
            $data = json_decode($json_data, true);

            $cart = Cart::getInstance();
            $cart->removeGoods($data);

            http_response(200, json_encode($response), 'application/json');
        } catch (Exception $e) {
            $response['result'] = 0;
            $response['errorMessage'] = $e->getMessage();
            http_response(400, json_encode($response), 'application/json');
        }
    }
}
