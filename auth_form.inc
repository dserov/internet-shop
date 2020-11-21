<?php
$title = "Авторизация";
?>
<div id='loginpanel'>
    <h1>Авторизация пользователя</h1>
    <br>
    <br>
    Подсказка. Админ - admin/admin123, Пользователь - user/user123
    <form action="?page=login" method="post">
        <label>Логин: <input size='32' class='textbox' type='text' name='login' autofocus></label>
        <label>Пароль: <input size='32' class='textbox' type='password' name='pass'></label>
        <input class='button' type='submit' value='Вход'>
    </form>
    <span><?= $message ?></span>
</div>
