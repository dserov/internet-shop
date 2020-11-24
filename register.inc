<?php
    if (isAdmin() || isUser()) {
        // залогиненому тут делать нечего
        header('Location: ?page=personal_area');
        exit;
    }
?>
<div id='loginpanel'>
    <h1>Регистрация пользователя</h1>
    <br>
    <br>
    <div id="register-ok">
        <p>Регистрация успешна! Теперь вы можете <a href="?page=login" title="Войти в систему">войти</a></p>
    </div>
    <div id="register">
        <form id="form-register" action="#" method="post">
            <fieldset>
                <div class="input_field">
                    <span>Ваше ФИО: </span>
                    <input size='32' class='textbox' type='text' name='fio' autofocus required>
                </div>
                <div class="input_field">
                    <span>Логин: </span>
                    <input size='32' class='textbox' type='text' name='login' autofocus required>
                </div>
                <div class="input_field">
                    <span>Пароль: </span>
                    <input size='32' class='textbox' type='text' name='password' required>
                </div>
                <div class="input_field">
                    <span>&nbsp;</span>
                    <input class='button' type='submit' value='Регистрация'>
                </div>
            </fieldset>
        </form>
    </div>
</div>

<script type="text/javascript" src="js/register.js"></script>
