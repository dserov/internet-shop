<?php

$product = DB::getInstance()->QueryOne("SELECT * from products WHERE id=?", $_GET['id']);
if (!$product) {
    ?><h1>Такой товар не найден!</h1><?php
    return;
}

// главное фото товара
$photos = [];
$rows = DB::getInstance()->QueryMany("SELECT * FROM pictures where product_id=? ORDER BY id", $product['id']);
foreach ($rows as $row) {
    $photos[] = [
        'picture_full' => WEB_ROOT . '/' . str_replace(DIRECTORY_SEPARATOR, '/', $row['path']) . $row['name'],
        'picture_thumb' => WEB_ROOT . '/' . str_replace(DIRECTORY_SEPARATOR, '/', $row['path'] . THUMBNAIL_DIR) . $row['name'],
        'picture_alt' => $row['alt']];
}

$title = $product['name'];

function showGallery($photos)
{
    if ( !$photos) {
        echo "<span>Фотогалеря еще не создана для этого товара</span>";
        return;
    }

    foreach ($photos as $photo) {
        $fullImageFile = ROOT_DIR . '\..' . str_replace('/', DIRECTORY_SEPARATOR, $photo['picture_full']);

        if (!is_file($fullImageFile) || !is_readable($fullImageFile)) {
            // Удалим отсутствующий
//            DB::getInstance()->QueryOne("DELETE FROM pictures where id=? limit 1", $picture['id']);
            continue;
        };
        echo sprintf('<a class="gallery-link" data-fancybox="gallery" href="%s">
            <img class="gallery-img" src="%s" width="%d" height="%d" alt="%s"></a>',
            $photo['picture_full'], $photo['picture_thumb'], THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, $photo['picture_alt']);
    }
}
?>
        <nav aria-label="breadcrumb">
            <ol class="breadcrumb">
                <li class="breadcrumb-item"><a href="<?= WEB_ROOT . '/?page=catalog' ?>">Каталог товаров</a></li>
                <li class="breadcrumb-item active" aria-current="page"><?= $product['name'] ?></li>
            </ol>
        </nav>
        <h1><?= $product['name'] ?></h1>
        <h3 class="karto4ka_tovara">Описание товара</h3>
        <a target="_blank" href="<?= $photos[0]['picture_full']?>" title="Открыть полную картинку">
            <img src="<?= $photos[0]['picture_thumb']?>" alt="10&quot; Ноутбук 4Good Cl100 синий" class="karto4ka_tovara_img_thumb">
        </a>
        <p class="karto4ka_tovara_desc_short karto4ka_tovara"><?= $product['description_short'] ?></p>
        <span class="cena_tovara clearfix"><strong>Всего за</strong> - <span class="summa"><?= $product['price'] ?></span></span>
        <button class="btn_kupit" onclick="confirm('Положить товар в корзину?')">Купить</button>
        <div class="clearfix"></div>
        <h3 class="karto4ka_tovara">Фотогалерея
            <?php if (isAdmin()) { ?>
                <a href="<?= '?page=photo&product_id='?><?= $product['id']?>">Редактировать</a>
            <?php } ?>
        </h3>
        <?php showGallery($photos) ?>
        <h3 class="karto4ka_tovara">Характеристики товара</h3>
        <?= $product['property'] ?>
        <h3 class="karto4ka_tovara">Подробное описание товара</h3>
        <p class="karto4ka_tovara_desc_full karto4ka_tovara"><?= $product['description'] ?>.</p>
