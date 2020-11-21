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
    if (isset($_SESSION['login']) && isset($_SESSION['pass'])) {
        $row = DB::getInstance()->QueryOne("SELECT * FROM users WHERE login=? and password=? LIMIT 1;", $_SESSION['login'], $_SESSION['pass']);
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
                if ($_POST['pass'] && SECRET_SALT . md5($_POST['pass']) . SECRET_SALT == $row['password']) {
                    $_SESSION['login'] = $row['login'];
                    $_SESSION['pass'] = $row['password'];
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
