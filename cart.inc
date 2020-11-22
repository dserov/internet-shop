<?php
/**
 * Created by PhpStorm.
 * User: MegaVolt
 * Date: 19.11.2020
 * Time: 14:57
 */


$goodsInCart = DB::getInstance()->QueryMany("SELECT g.id AS goods_id, g.name, SUM(g.price) AS summa, SUM(c.quantity) quantity 
        FROM cart c INNER JOIN goods g ON c.goods_id = g.id where c.user_id=? GROUP BY g.id", $_SESSION['user_id']);
?>
<h1>Корзина</h1>

<?php if (!$goodsInCart) { ?>
    <strong>Ваша корзина пуста</strong>
<?php } else { ?>
    <table class="tbl_harakteristiki">
        <tr>
            <th>Наименование</th>
            <th>Количество</th>
            <th>Сумма</th>
            <th>&nbsp;</th>
        </tr>
        <?php foreach ($goodsInCart as $good) {?>
        <tr>
            <td><?= $good['name']?></td>
            <td><?= $good['quantity']?></td>
            <td><?= $good['summa']?></td>
            <td><button data-product-id="<?= $good['goods_id']?>" class="btn-delete-good" title="Удалить товар из корзины">Удалить</button></td>
        </tr>
        <?php } ?>
    </table>
<?php } ?>

<script type="text/javascript">
    $(document).ready(function (e) {
        $(document).on("click", ".btn-delete-good", function (e) {
            if (!confirm("Удалить товар?")) return;
            let data = {id_product: $(this).data('product-id')};
            $.post("api.php/cart/remove/goods/",
                JSON.stringify(data),
                function (otvet, textStatus, jqXHR) {
                    show_message('Удалено', 'true');
                    // перегрузим страницу
                    window.location.reload();
                });
        })
    });
</script>
