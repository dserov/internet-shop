<?php
/**
 * Created by PhpStorm.
 * User: MegaVolt
 * Date: 23.11.2020
 * Time: 14:02
 */

// дальше для админа
if (!isAdmin()) {
    ?><h1>Эта страница для админа!</h1><?php
    return;
}

?>
<h1>Стол заказов</h1>
