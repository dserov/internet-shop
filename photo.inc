<?php

$title = "Фотогалерея товара";

$errors = [];
$uploadDir = 'foto' . DIRECTORY_SEPARATOR;

$productId = isset($_GET['product_id']) ? $_GET['product_id'] : null;

$product = DB::getInstance()->QueryOne("SELECT * FROM products WHERE id=?", $productId);

if ($_SERVER['REQUEST_METHOD'] == 'POST' && $_FILES) {
    processFiles($productId, $uploadDir, $errors);
}

function processFiles($productId, $uploadDir, &$errors)
{
    foreach ($_FILES as $file) {
        if ($file['error'] != 0) {
            $errors[] = sprintf('Файл "%s" не загрузился, код ошибки %d', $file['name'], $file['error']);
            continue;
        }
        $result = processFile($productId, $uploadDir, $file['tmp_name']);
        if (is_string($result)) {
            $errors[] = sprintf('Файл "%s" не загрузился, ошибка "%s"', $file['name'], $result);;
        }
    }
}

function processFile($productId, $uploadDir, $tmpName)
{
    // генерим уникальное имя
    $extension = getTrueExtension($tmpName);
    if ($extension === false)
        return 'Необрабатываемый тип файла';
    $newName = getUniqueName($uploadDir, $extension);
    if ($newName === false)
        return 'Не удалось получить уникальное имя файла';

    // переносим куда надо
    if (!move_uploaded_file($tmpName, $uploadDir . $newName)) {
        return 'Не удалось переместить/переименовать файл';
    }

    // создадим превьюшку
    if (resizeFile($uploadDir, $newName, $extension) === false)
        return 'Не удалось создать превьюшку';

    // сохраним в БД инфу
    $fileSize = filesize($uploadDir . $newName);
    DB::getInstance()->QueryOne("INSERT INTO pictures (path, `name`, `size`, `product_id`) values(?,?,?,?);",
        $uploadDir,
        $newName,
        (!$fileSize ? 0 : $fileSize),
        $productId
    );
}

/**
 * Ресайз картинки. Для упрощения кода не будем проверять существование функций и результат их вызова
 * и НЕ БУДЕМ ЗАМОРАЧИВАТЬСЯ С ГЕОМЕТРИЕЙ
 *
 * @param $uploadDir
 * @param $newName
 * @param $extension
 * @return mixed
 */
function resizeFile($uploadDir, $newName, $extension)
{
    $functions = [
        'jpg' => ['imagecreatefromjpeg', 'imagejpeg'],
        'png' => ['imagecreatefrompng', 'imagepng'],
        'gif' => ['imagecreatefromgif', 'imagegif'],
    ];
    // make resource
    $image = $functions[$extension][0]($uploadDir . $newName);
    $image = imagescale($image, THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT);
    // save picture
    return $functions[$extension][1]($image, $uploadDir . THUMBNAIL_DIR . $newName);
}

/**
 * Получим расширение по РЕАЛЬНОМУ содержимому файла
 *
 * @param $tmpName
 * @return bool|string
 */
function getTrueExtension($tmpName)
{
    $finfo = new finfo(FILEINFO_MIME_TYPE);
    $mimetype = $finfo->file($tmpName);
    switch ($mimetype) {
        case 'image/jpeg':
            return 'jpg';
        case 'image/png':
            return 'png';
        case 'image/gif':
            return 'gif';
    }
    return false;
}

/**
 * Получение уникального имени файла в заданном каталоге
 *
 * @param string $dir путь, где лежат фотки
 * @param string $ext требуемое расширение
 * @return bool|string
 */
function getUniqueName($dir, $ext)
{
    $i = 100;
    do {
        $tmpName = mb_substr(uniqid(), 0, 8) . ($ext ? "." . $ext : "");
        $i--;
    } while (file_exists($dir . $tmpName) && $i > 0);
    if ($i <= 0) {
        // за 100 попыток уникальное имя не получили
        return false;
    }
    return $tmpName;
}

/**
 * Форматируем строку с размером
 *
 * @param $bytes
 * @param int $precision
 * @return string
 */
function formatBytes($bytes, $precision = 2)
{
    $units = array('B', 'KB', 'MB', 'GB', 'TB');
    $bytes = max($bytes, 0);
    $pow = floor(($bytes ? log($bytes) : 0) / log(1024));
    $pow = min($pow, count($units) - 1);
    $bytes /= pow(1024, $pow);
    return round($bytes, $precision) . ' ' . $units[$pow];
}

function readGallery(&$errors, $productId)
{
    $pictures = DB::getInstance()->QueryMany("SELECT * FROM pictures where product_id=? ORDER BY id", $productId);
    if (!$pictures) {
        $errors[] = 'Галерея пуста';
        return;
    }
    foreach ($pictures as $picture) {
        $fullImageFile = $picture['path'] . DIRECTORY_SEPARATOR . $picture['name'];
        if (!is_file($fullImageFile) || !is_readable($fullImageFile)) {
            // Удалим отсутсвующий
            DB::getInstance()->QueryOne("DELETE FROM pictures where id=? limit 1", $picture['id']);
            continue;
        };
        // ссылка на превьюшку
        $thumbImageFile = $picture['path'] . THUMBNAIL_DIR . $picture['name'];
        if (!is_readable($thumbImageFile)) {
            $thumbImageFile = $fullImageFile;
        }

        // из путей сделаем ссылки
        $thumbImageFile = str_replace(DIRECTORY_SEPARATOR, '/', $thumbImageFile);
        echo sprintf('<a class="gallery-link" data-fancybox="gallery" href="?show=%d">
            <img class="gallery-img" src="%s" width="%d" height="%d" alt="%s">
            <p>Просмотров: %d</p>
            <p>Размер: %s</p></a>',
            $picture['id'], $thumbImageFile, THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT,
            $picture['alt'], $picture['click'], formatBytes($picture['size']));
    }
}

?>
<h1>Фотогалерея продукта</h1>
<h3><?= $product['name']?></h3>
<div class="clearfix"></div>
*Первая картинка станет главным изображением товара
<div class="photo-gallery">
<?php if ($productId) {
    readGallery($errors, $productId);
} else { ?>Код товара не задан<?php } ?>
</div>
<?php if ($errors) echo '<div class="error-list">' . implode('<br>', $errors) . '</div>' ?>
<form method="post" action="?page=photo&product_id=<?= $productId ?>" enctype="multipart/form-data">
    <h2>Загрузка новой картинки</h2>
    <input type="file" name="file-upload" id="file-upload"/>
    <button type="submit">Загрузить</button>
</form>
