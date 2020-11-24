<?php

/**
 *
 * @return bool
 * @throws Exception
 */
function CheckAuth(&$message = '')
{
    if (CheckAuthLogin($message)) {
        return true;
    }

    return CheckAuthPost($message);
}

/**
 * Проверка логина из сессии
 *
 * @param string $message
 * @return bool
 * @throws Exception
 */
function CheckAuthLogin(&$message = '')
{
    if (isset($_SESSION['login']) && isset($_SESSION['password'])) {
        $row = DB::getInstance()->QueryOne("SELECT * FROM users WHERE login=? and password=? LIMIT 1;", $_SESSION['login'], $_SESSION['password']);
        if (!$row) {
            session_destroy();
        } else {
            $_SESSION['userId'] = $row['id'];
            return true;
        }
    }
    $message = 'Не авторизован!';
    return false;
}

function CheckAuthPost(&$message)
{
    if ($_SERVER['REQUEST_METHOD'] == 'POST') {
        if (isset($_POST['login']) && $_POST['login'] != '') {
            $row = DB::getInstance()->QueryOne("SELECT * FROM users WHERE login=? LIMIT 1;", $_POST['login']);
            if (!$row) {
                $message = "Неверный логин или пароль.";
                return false;
            } else {
                if ($_POST['password'] && SECRET_SALT . md5($_POST['password']) . SECRET_SALT == $row['password']) {
                    $_SESSION['login'] = $row['login'];
                    $_SESSION['password'] = $row['password'];
                    $_SESSION['user_id'] = $row['id'];
                    $_SESSION['is_admin'] = $row['is_admin'];
                    $message = '';
                    return true;
                }
            }
        }
        $message = 'Неверный логин или пароль.';
    }
    return false;
}

function authRegister()
{
    try {
        $response['result'] = 1;

        $json_data = file_get_contents("php://input");
        $data = json_decode($json_data, true);

        if (!$data['login']) throw new Exception('Пустой логин');
        if (!$data['password']) throw new Exception('Пустой пароль');
        if (!$data['fio']) throw new Exception('Пустое ФИО');
        $user = DB::getInstance()->QueryOne("select * from users where login=?", $data['login']);
        if ($user) throw new Exception('Логин уже занят!');

        $password = SECRET_SALT . md5($data['password']) . SECRET_SALT;
        DB::getInstance()->QueryOne("insert into users (login, password, fio) values (?,?,?)", $data['login'], $password, $data['fio']);

        http_response(200, json_encode($response), 'application/json');
    } catch (Exception $e) {
        $response['result'] = 0;
        $response['errorMessage'] = $e->getMessage();
        http_response(400, json_encode($response), 'application/json');
    }
}