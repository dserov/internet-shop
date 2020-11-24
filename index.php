<?php

session_start();

require_once 'db.inc';
require_once 'functions.inc';
require_once 'auth.inc';

//if ($_SERVER['HTTPS'] != 'on') {
//    header("Location: https://" . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);
//    exit;
//}

$title = 'Главная страница';
$page = isset($_GET['page']) ? $_GET['page'] : '';

if ($page == 'logout') {
    if(session_id()) session_destroy();
    header('Location: ' . WEB_ROOT . '?');
    exit;
}

$message = '';
CheckAuth($message);
if ($page == 'login' && isAdmin()) {
    // только что авторизовался админ
    header('Location: ' . WEB_ROOT . '?page=goods');
    exit;
}

if ($page == 'login' && isUser()) {
    // только что авторизовался юзер
    header('Location: ' . WEB_ROOT . '?page=personal_area');
    exit;
}

// Буферизируем вывод, чтобы работало header(Location) в модулях
ob_start();

switch ($page) {
    case 'feedback':
        include 'feedback.inc';
        break;
    case 'photo':
        include 'photo.inc';
        break;
    case 'login':
        include 'login.inc';
        break;
    case 'goods_item':
        include 'goods_item.inc';
        break;
    case 'catalog':
        include 'catalog.inc';
        break;
    case 'contacts':
        include 'contacts.inc';
        break;
    case 'goods':
        include 'goods.inc';
        break;
    case 'cart':
        include 'cart.inc';
        break;
    case 'register':
        include 'register.inc';
        break;
    case 'personal_area':
        include 'personal_area.inc';
        break;
    default:
        include 'main_page.inc';
}

$content = ob_get_clean();

// пришлось сюда сунуть чтение корзины. В идеале сделать синглтоном тоже, как и операции с БД
$cartContent = readCart();
function readCart() {
    if (! (isUser() || isAdmin())) {
        return 'Для покупок необходима <a href="?page=login">авторизация</a>';
    }

    $goodsInCart = DB::getInstance()->QueryMany("SELECT g.id AS goods_id, g.name, SUM(g.price) AS summa, SUM(c.quantity) quantity 
        FROM cart c INNER JOIN goods g ON c.goods_id = g.id where c.user_id=? GROUP BY g.id", $_SESSION['user_id']);
    return "<a href='?page=cart' title='Смотреть корзину'>В корзине</a>: " . count($goodsInCart) . " товаров";
}

include 'header.inc';
echo $content;
include 'footer.inc';
