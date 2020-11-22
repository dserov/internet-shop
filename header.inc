<!doctype html>
<html lang="ru">
<head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css"
          integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/gh/fancyapps/fancybox@3.5.7/dist/jquery.fancybox.min.css"/>
    <link rel="stylesheet" href="<?= WEB_ROOT . '/css/style.css' ?>">

    <title><?= $title ?></title>
    <script src="https://code.jquery.com/jquery-3.5.1.min.js"></script>
</head>
<body>
<div class="container">
    <div class="content">
        <div class="header">
            <nav class="navbar navbar-expand-md d-flex align-items-end">
                <a class="navbar-brand" href="#">
                    <span class="d-md-none logo-text">AVALON</span>
                    <img class="d-none d-md-block" src="<?= WEB_ROOT . '/img/logo_small.jpg' ?>" alt="">
                </a>
                <button class="navbar-light navbar-toggler" type="button" data-toggle="collapse"
                        data-target="#navbarsExampleDefault"
                        aria-controls="navbarsExampleDefault" aria-expanded="false" aria-label="Toggle navigation">
                    <span class="navbar-toggler-icon"></span>
                </button>

                <div class="collapse navbar-collapse justify-content-end" id="navbarsExampleDefault">
                    <ul class="navbar-nav">
                        <li class="nav-item"><a class="nav-link" href="<?= WEB_ROOT . '/' ?>"
                                                title="Главная страница">Главная</a>
                        </li>
                        <li class="nav-item"><a class="nav-link" href="<?= WEB_ROOT . '/?page=catalog' ?>"
                                                title="Переход в каталог товаров">Каталог товаров</a></li>
                        <li class="nav-item"><a class="nav-link" href="<?= WEB_ROOT . '/?page=feedback' ?>"
                                                title="Отзывы о сайте">Отзывы</a>
                        </li>
                        <li class="nav-item"><a class="nav-link" href="<?= WEB_ROOT . '/?page=contacts' ?>"
                                                title="Контактные реквизиты">Контакты</a>
                        </li>
                    </ul>
                </div>
            </nav>
            <div class="user_panel">
                <?php if ($_SESSION && $_SESSION['user_id']) { ?>
                    <div>
                        Вы вошли, как <span><?= $_SESSION['login'] ?></span>
                        <a href="?page=personal_area">Личный кабинет</a>
                    </div>
                <?php } ?>
                <div>
                    <?= $cartContent ?>
                </div>
            </div>
        </div>
