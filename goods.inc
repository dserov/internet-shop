<?php
$title = "Список товаров";

// дальше для админа
if (!isAdmin()) {
    ?><h1>Эта страница для админа!</h1><?php
    return;
}

$errors = [];

$goods_item = [];
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $errors[] = doGoodsItemAction($goods_item);
}

// вывод уже имеющихся записей
try {
    $rows = DB::getInstance()->QueryMany("SELECT * FROM goods order by `name`");
} catch (Exception $e) {
    $error[] = $e->getMessage();
}

function doGoodsItemAction(&$goods_item)
{
    try {
        $error = '';
        $goods_item['action'] = isset($_POST['action']) ? $_POST['action'] : '';
        $goods_item['id'] = isset($_POST['id']) ? $_POST['id'] : null;
        $goods_item['name'] = isset($_POST['name']) ? $_POST['name'] : null;
        $goods_item['description_short'] = isset($_POST['description_short']) ? $_POST['description_short'] : null;
        $goods_item['property'] = isset($_POST['property']) ? $_POST['property'] : null;
        $goods_item['description'] = isset($_POST['description']) ? $_POST['description'] : null;
        $goods_item['price'] = isset($_POST['price']) ? $_POST['price'] : null;

        if ($goods_item['action'] == 'add') {
// add new goods_item
            goodsItemAdd($goods_item);
            if (!$error) {
// сделаем редирект, чтоб не добавить еще один товар при простом обновлении страницы
                header("Location: " . $_SERVER['REQUEST_URI']);
                exit;
            }
            return '';
        }

        if ($goods_item['action'] == 'delete') {
            goodsItemDelete($goods_item);
        }
        if ($goods_item['action'] == 'update') {
            goodsItemUpdate($goods_item);
            $goods_item = [];
        }

// переход в режим редактирования
        if ($goods_item['action'] == 'edit') {
            $goods_item = DB::getInstance()->QueryOne("select * from goods where id=? limit 1", $goods_item['id']);
            if (!$goods_item)
                throw new Exception('Товар не найден');
            $goods_item['action'] = 'edit';
        }
    } catch (Exception $e) {
        $error = $e->getMessage();
    }

    return $error;
}

/**
 * Обновить товар
 *
 * @param array $goods_item
 * @throws Exception
 */
function goodsItemUpdate($goods_item)
{
// добавление товара нужно больше проверок. Например на неотрицательность цены))
    if (empty($goods_item['id'])) throw new Exception('Не заполнен код');
    if (empty($goods_item['name'])) throw new Exception('Не заполнено наименование');

// "очищать" входные данные не будем, пусть этим БД займется при выполнении запросов
    DB::getInstance()->QueryOne("update goods set 
                                      `name`=?
                                    , `description_short`=?
                                    , `property`=?
                                    , `description`=?
                                    , `price`=?
                                     where id=?",
        $goods_item['name'],
        $goods_item['description_short'],
        $goods_item['property'],
        $goods_item['description'],
        $goods_item['price'],
        $goods_item['id']);
}

/**
 * Добавить товар
 *
 * @param array $goods_item
 * @throws Exception
 */
function goodsItemAdd($goods_item)
{
    if (empty($goods_item['name'])) throw new Exception('Не заполнено наименование');

    DB::getInstance()->QueryOne("insert into goods set 
                                      `name`=?
                                    , `description_short`=?
                                    , `property`=?
                                    , `description`=?
                                    , `price`=?",
        $goods_item['name'],
        $goods_item['description_short'],
        $goods_item['property'],
        $goods_item['description'],
        $goods_item['price']);
}

/**
 * Удаление товара
 *
 * @param $goods_item
 * @throws Exception
 */
function goodsItemDelete($goods_item)
{
    if (!$goods_item['id'])
        throw new Exception('Код удаляемого товара не задан');

    DB::getInstance()->QueryOne("delete from goods where id=? limit 1", $goods_item['id']);
}

?>

<span class="error"><?= implode('<br>', $errors) ?></span>
<h1>Список товаров</h1>
<table class="tbl_harakteristiki">
    <tr>
        <th>Наименование</th>
        <th>Цена</th>
        <?php if (isAdmin()) { ?>
            <th>Действия</th>
        <?php } ?>
    </tr>
    <?php if (!$rows) {
        echo "<tr><td colspan='3'>Товаров пока нет!</td></tr>";
    } else {
        foreach ($rows as $row) {
            ?>
            <tr>
                <td><?= $row['name'] ?></td>
                <td><?= $row['price'] ?></td>
                <?php if (isAdmin()) { ?>
                    <td class="feedback-actions__panel">
                        <form action="?page=goods" method="post"><input type="hidden" name="id"
                                                                           value="<?= $row['id'] ?>">
                            <button type="submit" name="action" value="delete" title="Удалить товар">Удалить</button>&nbsp;
                        </form>
                        <form action="?page=goods" method="post"><input type="hidden" name="id"
                                                                           value="<?= $row['id'] ?>">
                            <button type="submit" name="action" value="edit" title="Изменить товар">Изменить</button>&nbsp;
                        </form>
                        <a href="<?= '?page=photo&product_id=' ?><?= $row['id'] ?>">Фотогалерея</a>
                    </td>
                <?php } ?>
            </tr>
            <?php
        }
    } ?>
</table>
<hr>
<?php if ($goods_item['action'] == 'edit') { ?>
    <h2>Редактировать товар</h2>
<?php } else { ?>
    <h2>Добавить товар</h2>
<?php } ?>
<form action="?page=goods" method="post">
    <input type="hidden" name="id" value="<?= $goods_item['id'] ?>">
    <div class="input_field"><span>Наименование: </span><input type="text" name="name" value="<?= htmlspecialchars($goods_item['name']) ?>"
                                                      required placeholder="Введите наименование"></div>
    <div class="input_field"><span>Краткое описание: </span><textarea name="description_short"
                                                           placeholder="Краткое описание"><?= htmlspecialchars($goods_item['description_short']) ?></textarea>
    </div>
    <div class="input_field"><span>Характеристики: </span><textarea name="property"
                                                           placeholder="Таблица характеристик"><?= htmlspecialchars($goods_item['property']) ?></textarea>
    </div>
    <div class="input_field"><span>Описание: </span><textarea name="description"
                                                           placeholder="Подробное описание"><?= htmlspecialchars($goods_item['description']) ?></textarea>
    </div>
    <div class="input_field"><span>Цена: </span><input type="text" name="price" value="<?= $goods_item['price'] ?>"
                                                       placeholder="Цена"></div>
    <div class="input_field"><span>&nbsp;</span>
        <?php if ($goods_item['action'] == 'edit') { ?>
            <button type="submit" name="action" value="update">Обновить товар</button>
        <?php } else { ?>
            <button type="submit" name="action" value="add">Добавить товар</button>
        <?php } ?>
    </div>
</form>
