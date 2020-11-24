<?php
/**
 * Created by PhpStorm.
 * User: MegaVolt
 * Date: 19.11.2020
 * Time: 14:57
 */

$goodsInCart = Cart::getInstance()->getGoodsInCart();
$discount = checkDiscount();
if ($discount) {
    $data = [];
    foreach ($goodsInCart as $good) {
        $good['discount'] = round($good['price'] * $discount / 100, 2);
        $good['itogo'] = $good['price'] - $good['discount'];
        $good['vsego'] = round($good['itogo'] * $good['quantity'], 2);
        $data[] = $good;
    }
    $goodsInCart = $data;
}

/**
 * проверка на скидку. Возвращает процент скидки
 *
 * @return int
 */
function checkDiscount()
{
    return 5;
}

?>
<h1>Корзина</h1>

<?php if (!$goodsInCart) { ?>
    <strong>Ваша корзина пуста</strong>
<?php } else {
    $total = 0; ?>
    <form action="tra-la-la">
        <table class="tbl_harakteristiki" id="cart-table">
            <thead>
            <tr>
                <th>Наименование</th>
                <th>Количество</th>
                <th>Стоимость</th>
                <th>Скидка</th>
                <th>Стоимость с учетом скидки</th>
                <th>Всего</th>
                <th>&nbsp;</th>
            </tr>
            </thead>
            <tbody>

            <?php foreach ($goodsInCart as $good) { ?>
                <tr data-product-id="<?= $good['goods_id'] ?>">
                    <td><?= $good['name'] ?></td>
                    <td><input type="text" value="<?= $good['quantity'] ?>" name="quantity" class="cart-quantity"/></td>
                    <td><span class="cart-price"><?= sprintf("%.2f", $good['price']) ?></span></td>
                    <td><span class="cart-discount"><?= sprintf("%.2f", $good['discount']) ?></span></td>
                    <td><span class="cart-itogo"><?= sprintf("%.2f", $good['itogo']) ?></span></td>
                    <td><span class="cart-vsego"><?= sprintf("%.2f", $good['vsego']) ?></span></td>
                    <td>
                        <button data-product-id="<?= $good['goods_id'] ?>" class="btn-delete-good"
                                title="Удалить товар из корзины">Удалить
                        </button>
                    </td>
                </tr>
                <?php $total += $good['vsego'];
            } ?>
            </tbody>
        </table>
        <p>Общая сумма: <span class="cart-total"><?= sprintf("%.2f", round($total, 2)) ?></span></p>
        <br>
        <button type="submit">Создать заказ</button>
    </form>
<?php } ?>

<script type="text/javascript">
    $(document).ready(function (e) {
        $(document).on("click", ".btn-delete-good", function (e) {
            e.preventDefault();
            if (!confirm("Удалить товар?")) return;
            let data = {id_product: $(this).data('product-id')};
            $.post("api.php/cart/remove/goods/",
                JSON.stringify(data),
                function (otvet, textStatus, jqXHR) {
                    show_message('Удалено', 'true');
                    // перегрузим страницу
                    window.location.reload();
                }
            );
        });
        let options = {
            minValue: 1,
            updateHandler: function () {
                let data = {id_product: 0, quantity: parseInt(this.val())};
                let $tr = this.closest('tr');
                data.id_product = $tr.data('product-id');
                data.action = 'set'; // флаг установки(!) количества
                $.post("api.php/cart/update/goods/",
                    JSON.stringify(data),
                    function (otvet, textStatus, jqXHR) {
                        // пересчет строк таблицы
                        recalcCart();
                        show_message('Сохранено', 'true');
                    }
                )
            }
        };
        makeSpinner('input.cart-quantity', options);
    });

    function recalcCart() {
        let $trList = $('#cart-table').find('tbody > tr');
        let totalSumma = 0;
        $.each($trList, function (idx, tr) {
            itogo = parseFloat($(tr).find('.cart-itogo').text());
            quantity = parseInt($(tr).find('.cart-quantity').val());
            $(tr).find('.cart-quantity').text((itogo*quantity).toFixed(2));
            totalSumma += itogo * quantity;
        });
        $('.cart-total').text(totalSumma.toFixed(2));
    }

</script>
