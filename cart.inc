<?php
/**
 * Created by PhpStorm.
 * User: MegaVolt
 * Date: 19.11.2020
 * Time: 14:57
 */

$goodsInCart = Cart::getInstance()->getGoodsInCartApplyDiscount(new Discount());
?>
<h1>Корзина</h1>

<div id="cart-message" class="cart-message">
    <h3>Ваша заявка принята!</h3>
    <p>Ожидайте звонка менеджера для подтвержения.</p>
    <p>Ваши заявки и их статус отображаются в <a href="?page=personal_area">личном кабинете</a>.</p>
    <p>Также проверьте адрес доставки.</p>
    <p>Спасибо, что доверяете нам!</p>
</div>
<?php if (!$goodsInCart) { ?>
    <strong>Ваша корзина пуста</strong>
<?php } else {
    $total = 0; ?>
    <form id="create-order" action="tra-la-la">
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
                let $tr = this.closest('tr');
                let data = {id_product: $tr.data('product-id'), quantity: parseInt(this.val()), action: "set"}; // флаг установки(!) количества
                let posting = $.post("api.php/cart/update/goods/",
                    JSON.stringify(data),
                    function (otvet, textStatus, jqXHR) {
                        // пересчет строк таблицы
                        recalcCart();
//                        show_message('Сохранено', 'true');
                    }
                );
                posting.fail(function () {
                    setTimeout(function () {
                        window.location.reload();
                    }, 10000);
                });
            }
        };
        makeSpinner('input.cart-quantity', options);

        $(document).on("submit", "#create-order", function (e) {
            e.preventDefault();
            if (!confirm("Создать заказ?")) return;
            $.post("api.php/orders/create_from_cart/",
                JSON.stringify({}),
                function (otvet, textStatus, jqXHR) {
                    $('#create-order').fadeOut();
                    $('.cart-message').fadeIn();
                }
            );
        });
    });

    function recalcCart() {
        let $trList = $('#cart-table').find('tbody > tr');
        let totalSumma = 0;
        $.each($trList, function (idx, tr) {
            itogo = parseFloat($(tr).find('.cart-itogo').text());
            quantity = parseInt($(tr).find('.cart-quantity').val());
            $(tr).find('.cart-vsego').text((itogo * quantity).toFixed(2));
            totalSumma += itogo * quantity;
        });
        $('.cart-total').text(totalSumma.toFixed(2));
    }

</script>
