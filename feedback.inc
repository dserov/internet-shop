<?php

$title = "Отзывы";
$errors = [];

$feedback = [];
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $errors[] = doFeedBackAction($feedback);
}

// вывод уже имеющихся записей
try {
    $rows = DB::getInstance()->QueryMany("SELECT * FROM reviews order by create_date desc");
} catch (Exception $e) {
    $error[] = $e->getMessage();
}

function doFeedBackAction(&$feedback)
{
    try {
        $error = '';
        $feedback['action'] = isset($_POST['action']) ? $_POST['action'] : '';
        $feedback['id'] = isset($_POST['id']) ? $_POST['id'] : null;
        $feedback['author'] = isset($_POST['author']) ? $_POST['author'] : null;
        $feedback['text'] = isset($_POST['text']) ? $_POST['text'] : null;

        if ($feedback['action'] == 'add') {
            // add new feedback
            feedbackAdd($feedback);
            if (!$error) {
                // сделаем редирект, чтоб не добавить еще один отзыв при простом обновлении страницы
                header("Location: " . $_SERVER['REQUEST_URI']);
                exit;
            }
            return '';
        }

        // дальше для админа
        if (!isAdmin()) {
            throw new Exception('Это действие только для админа');
        }

        if ($feedback['action'] == 'delete') {
            feedbackDelete($feedback);
        }
        if ($feedback['action'] == 'update') {
            feedbackUpdate($feedback);
            $feedback = [];
        }

        // переход в режим редактирования
        if ($feedback['action'] == 'edit') {
            $feedback = DB::getInstance()->QueryOne("select * from reviews where id=? limit 1", $feedback['id']);
            if (!$feedback)
                throw new Exception('Отзыв не найден');
            $feedback['action'] = 'edit';
        }
    } catch (Exception $e) {
        $error = $e->getMessage();
    }

    return $error;
}

/**
 * Обновить отзыв
 *
 * @param array $feedback
 * @throws Exception
 */
function feedbackUpdate($feedback)
{
    // добавление отзыва
    if (empty($feedback['id']) || empty($feedback['author']) || empty($feedback['text']))
        throw new Exception('Не верно заполнены поля формы');

    // "очищать" входные данные не будем, пусть этим БД займется при выполнении запросов
    DB::getInstance()->QueryOne("update reviews set `author`=?, `text`=?, create_date=now() where id=?",
        $feedback['author'], $feedback['text'], $feedback['id']);
}

/**
 * Добавить отзыв
 *
 * @param array $feedback
 * @throws Exception
 */
function feedbackAdd($feedback)
{
    // добавление отзыва
    if (empty($feedback['author']) || empty($feedback['text']))
        throw new Exception('Не верно заполнены поля формы');

    // "очищать" входные данные не будем, пусть этим БД займется при выполнении запросов
    $row = DB::getInstance()->QueryOne("select * from reviews where `author`=? and `text`=?", $feedback['author'], $feedback['text']);
    if ($row)
        throw new Exception('Такой отзыв уже есть! Перестаньте обновлять страницу))');

    DB::getInstance()->QueryOne("insert into reviews (`author`, `text`) values (?,?)", $feedback['author'], $feedback['text']);
}

/**
 * Удаление отзыва
 *
 * @param $feedback
 * @throws Exception
 */
function feedbackDelete($feedback)
{
    if (!$feedback['id'])
        throw new Exception('Код удаляемого отзыва не задан');

    DB::getInstance()->QueryOne("delete from reviews where id=? limit 1", $feedback['id']);
}

?>

<span class="error"><?= implode('<br>', $errors) ?></span>
<h1>Отзывы</h1>
<table class="reviews-table">
    <tr>
        <th>Автор</th>
        <th>Содержимое</th>
        <th>Добавлен</th>
        <?php if (isAdmin()) { ?>
            <th>Действия</th>
        <?php } ?>
    </tr>
    <?php if (!$rows) {
        echo "<tr><td colspan='3'>Отзывов пока нет!</td></tr>";
    } else {
        foreach ($rows as $row) {
            ?>
            <tr>
                <td><?= $row['author'] ?></td>
                <td><?= $row['text'] ?></td>
                <td><?= $row['create_date'] ?></td>
                <?php if (isAdmin()) { ?>
                    <td class="feedback-actions__panel">
                        <form action="?page=feedback" method="post"><input type="hidden" name="id"
                                                                           value="<?= $row['id'] ?>">
                            <button type="submit" name="action" value="delete" title="Удалить отзыв">Удалить</button>&nbsp;
                        </form>
                        <form action="?page=feedback" method="post"><input type="hidden" name="id"
                                                                           value="<?= $row['id'] ?>">
                            <button type="submit" name="action" value="edit" title="Изменить отзыв">Изменить</button>&nbsp;
                        </form>
                    </td>
                <?php } ?>
            </tr>
            <?php
        }
    } ?>
</table>
<hr>
<?php if ($feedback['action'] == 'edit') { ?>
    <h2>Редактировать отзыв</h2>
<?php } else { ?>
    <h2>Новый отзыв</h2>
<?php } ?>
<form action="?page=feedback" method="post">
    <input type="hidden" name="id" value="<?= $feedback['id'] ?>">
    <div class="input_field"><span>Имя: </span><input type="text" name="author" value="<?= $feedback['author'] ?>"
                                                      required placeholder="Введите имя"></div>
    <div class="input_field"><span>Отзыв: </span><textarea name="text" required
                                                           placeholder="Ваш отзыв"><?= $feedback['text'] ?></textarea>
    </div>
    <div class="input_field"><span>&nbsp;</span>
        <?php if ($feedback['action'] == 'edit') { ?>
            <button type="submit" name="action" value="update">Обновить отзыв</button>
        <?php } else { ?>
            <button type="submit" name="action" value="add">Добавить отзыв</button>
        <?php } ?>
    </div>
</form>
