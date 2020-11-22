<?php
/**
 * Created by PhpStorm.
 * User: MegaVolt
 * Date: 21.11.2020
 * Time: 22:24
 */

$user = DB::getInstance()->QueryOne("select * from users where id=?", $_SESSION['user_id']);
?>
<h1>Личный кабинет</h1>

<p>Приветствую вас, <?= $user['fio']?>!</p>
<p>Ваш логин в системе - <?= $user['login']?></p>

<p>Вы можете <a href="?page=logout" title="Выйти из системы">выйти</a></p>

