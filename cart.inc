<?php
/**
 * Created by PhpStorm.
 * User: MegaVolt
 * Date: 19.11.2020
 * Time: 14:57
 */

function readCart() {
    if (! (isUser() || isAdmin())) {
        return 'Для покупок необходима <a href="?page=login">авторизация</a>';
    }

    return 'Корзина: 4 товара';
}
