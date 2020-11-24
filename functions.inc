<?php
/**
 * Created by PhpStorm.
 * User: MegaVolt
 * Date: 16.11.2020
 * Time: 16:06
 */

define('ROOT_DIR', $_SERVER['DOCUMENT_ROOT']);
define('WEB_ROOT', '');
define('DS', DIRECTORY_SEPARATOR);

define('THUMBNAIL_DIR', 'thumb' . DIRECTORY_SEPARATOR);
define('THUMBNAIL_WIDTH', 128);
define('THUMBNAIL_HEIGHT', 128);

define('SECRET_SALT', 'a$pG0&32*G!Hs');

function isAdmin() {
    return isset($_SESSION) && $_SESSION['is_admin'] == '1';
}

function isUser() {
    return isset($_SESSION) && $_SESSION['is_admin'] == '0';
}

function http_header_code_verbose($code) {
    switch($code):
        case 200: return '200 OK';
        case 201: return '201 Created';
        case 204: return '204 No Content';
        case 205: return '205 Reset Content';
        case 400: return '400 Bad Request';
        case 401: return '401 Unauthorized';
        case 403: return '403 Forbidden';
        case 404: return '404 Not Found';
        case 405: return '405 Method Not Allowed';
        case 416: return '416 Requested Range Not Satisfiable';
        case 418: return "418 I'm a teapot";
        case 422: return '422 Unprocessable Entity';
        default:  return '500 Internal Server Error';
    endswitch;
}

function http_response($code,$content=false,$contentType='text/html',$charset='UTF-8') {

    header('HTTP/1.1 '.http_header_code_verbose($code));
    header('Status: '.http_header_code_verbose($code)."\r\n");
    header("Connection: Close\r\n");
    $ct = "Content-Type: $contentType";
    if ($charset)
        $ct .= "; charset=$charset";
    header($ct);
    if ($content) {
        header('Content-Length: '.strlen($content)."\r\n\r\n");
        print $content;
        exit;
    }
}

/**
 * Функция возвращает только 404-ю страницу
 */
function http_response_err404() {
    $content = file_get_contents(ROOT_DIR . DS . 'err404.html');
    http_response(404, $content); // в функции вызывается exit
    exit; // сюда исполнение не дойдет никогда
}

