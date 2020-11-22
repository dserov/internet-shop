<?php

session_start();

require_once 'db.inc';
require_once 'functions.inc';
require_once 'auth.inc';
require_once 'AltoRouter.class.php';
require_once 'cart_api.inc';

//------------------------------------------------------------------------------

//if ($_SERVER['HTTPS'] != 'on') {
//    header("Location: https://" . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);
//    exit;
//}

$response = array('result' => 0, 'errorMessage' => 'Не реализовано');
$skipAuth = $_SERVER['REQUEST_URI'] === '/api.php/auth/register/';

if (!CheckAuth() && !$skipAuth) {
    $response['errorMessage'] = 'Требуется авторизация!!!';
    $response['errorMessage'] = print_r($_SERVER['REQUEST_URI'], true);
    http_response(401, json_encode($response), 'application/json');
    return;
}

try {
    $router = new AltoRouter();
    $router->setBasePath(WEB_ROOT . '/api.php');

    $router->map("POST", "/cart/add/goods/",    "cartAddGoods");
    $router->map("POST", "/cart/remove/goods/", "cartRemoveGoods");
    $router->map("POST", "/auth/register/",     "authRegister");

    $match = $router->match();

    // call closure or throw 404 status
    if( is_array($match) && is_callable( $match['target'] ) ) {
        call_user_func_array( $match['target'], $match['params'] );
    } else {
        // no route was matched
        http_response(405, json_encode($response), 'application/json');
    }
} catch (Exception $e) {
    $response['errorMessage'] = $e->getMessage();
    http_response(400, json_encode($response), 'application/json');
}
