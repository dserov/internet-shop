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

$orders = Orders::getInstance()->getOrders();
$order_status = DB::getInstance()->QueryMany("SELECT * FROM order_status");
?>
<h1>Стол заказов</h1>
<?php if (!$orders) { ?>
    <strong>Заказов пока нет...</strong>
<?php } else { ?>
    <table class="tbl_harakteristiki" id="user-orders-table">
        <thead>
        <tr>
            <th>Логин<br>пользователя</th>
            <th>Номер</th>
            <th>Дата-время</th>
            <th>Сумма</th>
            <th>Статус</th>
            <th>&nbsp;</th>
        </tr>
        </thead>
        <tbody>
        <?php foreach ($orders as $order) { ?>
            <tr>
                <td><?= $order['user_login'] ?></td>
                <td><?= $order['id'] ?></td>
                <td><?= $order['order_date'] ?></td>
                <td><?= sprintf("%.2f", $order['vsego']) ?></td>
                <td><select class="order-status" data-order-id="<?= $order['id'] ?>">
                        <?php foreach ($order_status as $status) {
                            $selected = ($order['status_id'] == $status['id'] ? 'selected' : ''); ?>
                            <option value="<?= $status['id'] ?>" <?= $selected ?>><?= $status['name'] ?></option>
                        <?php } ?>
                    </select></td>
                <td>
                    <button class="show-order-detail" data-order-id="<?= $order['id'] ?>">Состав заказа</button>
                </td>
            </tr>
        <?php } ?>
        </tbody>
    </table>
<?php } ?>

<!-- Modal -->
<div id="order-detail" class="modal hide fade" tabindex="-1" role="dialog">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title">Состав заказа</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body">
                <table class="tbl_harakteristiki" id="order-detail-table">
                    <thead>
                    <tr>
                        <th>Наименование</th>
                        <th>Количество</th>
                        <th>Стоимость</th>
                        <th>Скидка</th>
                        <th>Стоимость с учетом скидки</th>
                        <th>Всего</th>
                    </tr>
                    </thead>
                    <tbody>
                    </tbody>
                </table>
            </div>
        </div>
    </div>
</div>

<script src="js/tmpl.min.js"></script>
<script type="text/x-tmpl" id="order_detail_row">
    <tr>
        <td>{%=o.name%}</td>
        <td>{%=o.quantity%}</td>
        <td>{%=o.price%}</td>
        <td>{%=o.discount%}</td>
        <td>{%=o.itogo%}</td>
        <td>{%=o.vsego%}</td>
    </tr>

</script>
<script type="text/javascript">
    $(document).ready(function (e) {
        $(document).on('click', '.show-order-detail', function (e) {
            let order_id = parseInt($(this).data('order-id'));
            $.post("api.php/orders/" + order_id + "/",
                JSON.stringify({}),
                function (otvet, textStatus, jqXHR) {
                    // очистить старые строки
                    let $tbody = $('#order-detail-table > tbody');
                    $tbody.find('tr').remove();
                    // есть ли данные?
                    if (!otvet.data || !otvet.data.detail) {
                        show_message('Детали заказа не получены! :(', 'false');
                        return;
                    }
                    // рендеринг новых строк с помощью шаблона https://github.com/blueimp/JavaScript-Templates
                    for (let key in otvet.data.detail) {
                        let row = otvet.data.detail[key];
                        let render = tmpl('order_detail_row', row);
                        $tbody.append(render);
                    }
                    // смотрим красоту
                    $('#order-detail').modal();
                }
            );
        })
        $(document).on('change', '.order-status', function (e) {
            let order_id = $(this).data('order-id');
            let status_id = parseInt($(this).val());
            let posting = $.post("api.php/orders/" + order_id + "/update/",
                JSON.stringify({status_id: status_id}),
                function (otvet, textStatus, jqXHR) {
                    show_message('Сохранено', 'true');
                }
            );
            posting.fail(function () {
                setTimeout(function () {
                    window.location.reload();
                }, 1000);
            });
        })
    })
</script>
