<?php

session_start();

require_once 'db.inc';
require_once 'functions.inc';
require_once 'auth.inc';
require_once 'cart.inc';

//if ($_SERVER['HTTPS'] != 'on') {
//    header("Location: https://" . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);
//    exit;
//}

$title = 'Главная страница';
$page = isset($_GET['page']) ? $_GET['page'] : '';

if ($page == 'logout') {
    if(session_id()) session_destroy();
    header('Location: ' . WEB_ROOT . '/');
    exit;
}

$message = '';
CheckAuth($message);
if ($page == 'login' && isAdmin()) {
    // только что авторизовался админ
    header('Location: ' . WEB_ROOT . '/admin/');
    exit;
}

if ($page == 'login' && isUser()) {
    // только что авторизовался юзер
    header('Location: ' . WEB_ROOT . '/');
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
        include 'auth_form.inc';
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
    default:
        include 'main_page.inc';
}

$content = ob_get_clean();

include 'header.inc';
echo $content;
include 'footer.inc';
