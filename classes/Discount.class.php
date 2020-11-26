<?php
/**
 * Created by PhpStorm.
 * User: MegaVolt
 * Date: 25.11.2020
 * Time: 10:25
 */

/**
 * Class Discount
 *
 * Тут работа со скидками, по коду товара, по дате-времени, дню недели и т.п.
 */
class Discount
{
    /**
     * Заглушка функции проверки на скидку. Возвращает процент скидки
     *
     * @param int $product_id Расчет скидки по коду товара, если есть
     * @param string $discount_message
     * @return int
     */
    public function checkDiscount(int $product_id = 0, string &$discount_message = ''): int
    {
        //
        $discount_message = 'Перманентная скидка на всё 5%!';
        return 5;
    }
}
