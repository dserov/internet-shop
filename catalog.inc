<?php
$title = "Каталог товаров";

$goods = DB::getInstance()->QueryMany("SELECT * from goods");
if ($goods) {
    // массив массивов, где ключем будет код продукта, значение - массив фото
    $thumbnails = [];
    // нужно всего одно фото.
    $rows = DB::getInstance()->QueryMany("SELECT * FROM pictures ORDER BY product_id, id desc");
    foreach ($rows as $row) {
        $thumbnails[$row['product_id']] = [
            'picture_path' => WEB_ROOT  . '/' . str_replace(DIRECTORY_SEPARATOR, '/', $row['path']) . $row['name'],
            'picture_thumb' => WEB_ROOT . '/' . str_replace(DIRECTORY_SEPARATOR, '/', $row['path'] . THUMBNAIL_DIR) . $row['name']];
    }
}

?>
        <h1>Каталог</h1>
        <div class="tovar_list clearfix d-md-flex">
            <?php foreach ($goods as $product) { ?>
                <div class="card">
                    <a href="<?= "?page=goods_item&id=" . $product['id'] ?>">
                        <img class="card-img-top mt-3 mb-sm-3" src="<?=$thumbnails[$product['id']]['picture_path'] ?>" alt="<?= htmlspecialchars($product['name']) ?>" width="128" height="128">
                        <div class="card-body">
                            <h5 class="card-title"><?= htmlspecialchars($product['name']) ?></h5>
                            <p>Цена: <?= $product['price'] ?></p>
                            <a href="<?= "?page=goods_item&id=" . $product['id'] ?>" class="btn btn-primary">Подробнее</a>
                        </div>
                    </a>
                </div>
            <?php } ?>
        </div>
        <?php if (isAdmin()) echo '<a href="?page=goods_item">Добавить новый товар</a>' ?>

