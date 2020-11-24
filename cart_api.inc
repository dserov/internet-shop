<?php
/**
 * Created by PhpStorm.
 * User: MegaVolt
 * Date: 21.11.2020
 * Time: 8:21
 */

/**
 * @param int $product_id
 */
function cartAddGoods() {
    $response['result'] = 1;
    try {
        $json_data = file_get_contents("php://input");
        $data = json_decode($json_data, true);

        // проверка входных данных
        $product = DB::getInstance()->QueryOne("select * from goods where id=?", $data['id_product']);
        if (!$product) throw new Exception('Такой продукт не найден');
        if (!is_numeric($data['quantity']) || $data['quantity'] <= 0)
            throw new Exception('Количество указано неверно');
        if (!$_SESSION['user_id']) throw new Exception('Не авторизован!');

        DB::getInstance()->QueryOne("INSERT INTO cart (goods_id, user_id, quantity) values (?,?,?)", $product['id'], $_SESSION['user_id'], $data['quantity']);

        http_response(200, json_encode($response), 'application/json');
    } catch (Exception $e) {
        $response['result'] = 0;
        $response['errorMessage'] = $e->getMessage();
        http_response(400, json_encode($response), 'application/json');
    }
}

function cartRemoveGoods() {
    $response['result'] = 1;
    try {
        $json_data = file_get_contents("php://input");
        $data = json_decode($json_data, true);

        // проверка входных данных
        if (!$_SESSION['user_id']) throw new Exception('Не авторизован!');

        DB::getInstance()->QueryOne("DELETE FROM cart WHERE goods_id=? and user_id=?", $data['id_product'], $_SESSION['user_id']);

        http_response(200, json_encode($response), 'application/json');
    } catch (Exception $e) {
        $response['result'] = 0;
        $response['errorMessage'] = $e->getMessage();
        http_response(400, json_encode($response), 'application/json');
    }
}

