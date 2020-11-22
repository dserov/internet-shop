<?php
$title = "Авторизация";
?>
<div id='loginpanel'>
    <h1>Авторизация пользователя</h1>
    <br>
    <br>
    Подсказка. Админ - admin/admin123, Пользователь - user/user123
    <form action="?page=login" method="post">
        <fieldset>
            <div class="input_field">
                <span>Логин: </span>
                <input size='32' class='textbox' type='text' name='login' autofocus required>
            </div>
            <div class="input_field">
                <span>Пароль: </span>
                <input size='32' class='textbox' type='password' name='password' required>
            </div>
            <div class="input_field">
                <span>&nbsp;</span>
                <input class='button' type='submit' value='Вход'>
            </div>
        </fieldset>
    </form>
    <span><?= $message ?></span>
    <p>Доступна <a href="?page=register" title="Регистрация нового пользователя">регистрация</a></p>
</div>
