--
-- Скрипт сгенерирован Devart dbForge Studio for MySQL, Версия 7.2.58.0
-- Домашняя страница продукта: http://www.devart.com/ru/dbforge/mysql/studio
-- Дата скрипта: 20.11.2020 22:20:37
-- Версия сервера: 5.5.5-10.3.22-MariaDB
-- Версия клиента: 4.1
--


--
-- Описание для базы данных internet_shop
--
DROP DATABASE IF EXISTS internet_shop;
CREATE DATABASE IF NOT EXISTS internet_shop
	CHARACTER SET utf8mb4
	COLLATE utf8mb4_general_ci;

-- 
-- Отключение внешних ключей
-- 
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;

-- 
-- Установить режим SQL (SQL mode)
-- 
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- 
-- Установка кодировки, с использованием которой клиент будет посылать запросы на сервер
--
SET NAMES 'utf8';

-- 
-- Установка базы данных по умолчанию
--
USE internet_shop;

--
-- Описание для таблицы pictures
--
CREATE TABLE IF NOT EXISTS pictures (
  id INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,
  path VARCHAR(50) NOT NULL,
  name VARCHAR(50) NOT NULL,
  size INT(11) UNSIGNED NOT NULL,
  click INT(11) UNSIGNED NOT NULL DEFAULT 0,
  alt VARCHAR(255) DEFAULT 'NULL',
  product_id INT(11) UNSIGNED DEFAULT NULL COMMENT 'Код продукта, куда относится картинка',
  thumbnail INT(11) DEFAULT 0 COMMENT 'Признак, что это главная картинка',
  PRIMARY KEY (id)
)
ENGINE = INNODB
AUTO_INCREMENT = 7
AVG_ROW_LENGTH = 5461
CHARACTER SET utf8mb4
COLLATE utf8mb4_general_ci
ROW_FORMAT = DYNAMIC;

--
-- Описание для таблицы products
--
CREATE TABLE IF NOT EXISTS products (
  id INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,
  name VARCHAR(255) NOT NULL,
  description_short TEXT DEFAULT NULL,
  thumbnail VARCHAR(255) DEFAULT 'NULL',
  property TEXT DEFAULT NULL,
  description TEXT DEFAULT NULL,
  price DOUBLE DEFAULT NULL,
  PRIMARY KEY (id)
)
ENGINE = INNODB
AUTO_INCREMENT = 3
AVG_ROW_LENGTH = 8192
CHARACTER SET utf8mb4
COLLATE utf8mb4_general_ci
ROW_FORMAT = DYNAMIC;

--
-- Описание для таблицы reviews
--
CREATE TABLE IF NOT EXISTS reviews (
  id INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,
  author VARCHAR(255) NOT NULL COMMENT 'автор',
  text TEXT NOT NULL COMMENT 'текст отзыва',
  create_date TIMESTAMP NOT NULL DEFAULT 'current_timestamp()' COMMENT 'дата создания',
  PRIMARY KEY (id)
)
ENGINE = INNODB
AUTO_INCREMENT = 5
AVG_ROW_LENGTH = 8192
CHARACTER SET utf8mb4
COLLATE utf8mb4_general_ci
ROW_FORMAT = DYNAMIC;

--
-- Описание для таблицы users
--
CREATE TABLE IF NOT EXISTS users (
  id INT(11) NOT NULL AUTO_INCREMENT,
  login VARCHAR(255) NOT NULL,
  password VARCHAR(255) DEFAULT 'NULL',
  fio VARCHAR(255) DEFAULT 'NULL',
  is_admin INT(11) DEFAULT 0,
  PRIMARY KEY (id)
)
ENGINE = INNODB
AUTO_INCREMENT = 3
AVG_ROW_LENGTH = 8192
CHARACTER SET utf8mb4
COLLATE utf8mb4_general_ci
ROW_FORMAT = DYNAMIC;

-- 
-- Вывод данных для таблицы pictures
--

/*!40000 ALTER TABLE pictures DISABLE KEYS */;
INSERT INTO pictures VALUES
(1, 'foto\\', '5fb81579.jpg', 198307, 0, NULL, 1, 0),
(2, 'foto\\', '5fb8157e.jpg', 158333, 0, NULL, 1, 0),
(3, 'foto\\', '5fb81581.jpg', 146557, 0, NULL, 1, 0),
(4, 'foto\\', '5fb815c4.jpg', 185095, 0, NULL, 2, 0),
(5, 'foto\\', '5fb815c8.jpg', 190431, 0, NULL, 2, 0),
(6, 'foto\\', '5fb815cc.jpg', 257377, 0, NULL, 2, 0);

/*!40000 ALTER TABLE pictures ENABLE KEYS */;

-- 
-- Вывод данных для таблицы products
--

/*!40000 ALTER TABLE products DISABLE KEYS */;
INSERT INTO products VALUES
(1, '10" Ноутбук 4Good Cl100 синий', 'Ноутбук 4Good Cl100 станет для вас великолепным выбором, если вам необходим мобильный компьютер, обладающий предельно компактными размерами. Корпус модели характеризуется габаритами 185×280×28 мм. Невысокая масса ноутбука (она равна лишь 1056 г) позволит вам постоянно носить устройство с собой, не испытывая неудобств. Ноутбук поместится даже в папку с документами, которую, в свою очередь, вы сможете носить в небольшом портфеле. 10-дюймовый экран устройства имеет разрешение 1024×600.', '5fb27672.jpg', '<table class="tbl_harakteristiki">\r\n            <tr>\r\n                <td>Операционная система</td>\r\n                <td>Windows 10</td>\r\n            </tr>\r\n            <tr>\r\n                <td>Диагональ экрана</td>\r\n                <td>10&quot;</td>\r\n            </tr>\r\n            <tr>\r\n                <td>Процессор</td>\r\n                <td>Intel Atom</td>\r\n            </tr>\r\n            <tr>\r\n                <td>Размер оперативной памяти</td>\r\n                <td>2 ГБ</td>\r\n            </tr>\r\n            <tr>\r\n                <td>Жесткий диск</td>\r\n                <td>(SSD) 32 Гб</td>\r\n            </tr>\r\n        </table>', 'Ноутбук 4Good Cl100 работает под управлением удобной в использовании операционной системы Windows 10. Вы будете обеспечены широкой совместимостью мобильного компьютера с различным периферийным оборудованием: сканерами, принтерами, многофункциональными устройствами, веб-камерами и другой техникой. Аппаратную основу ноутбука составляют 4-ядерный процессор Atom Z3735F, 2 ГБ оперативной памяти DDR3L и встроенный видеоадаптер Intel HD. Для размещения данных используется скоростной 32-гигабайтный SSD-накопитель. Ноутбук 4Good Cl100 оборудован модулем Wi-Fi и Bluetooth-интерфейсом. В наличии 2 порта USB 2.0. Для вывода изображения используется порт HDMI, подходящий для подключения к телевизорам.', 6999),
(2, '11.6" Ноутбук Irbis NB31 белый', 'Заниматься веб-серфингом, просматривать медиаконтент, играть и общаться с помощью ноутбука Irbis NB31 — одно удовольствие. Диагональ его экрана составляет 11.6″, разрешение 1366×768. Встроенная видеокарта Intel HD. 4-х ядерный процессор, а оперативная память типа DDR3 на 2 ГБ. Накопитель SSD — 32 ГБ.Два порта USB 2.0, встроенная веб-камера. Время автономной работы — 3 ч 30 мин.', '5fb27679.jpg', '<table class="tbl_harakteristiki">\r\n                <tbody><tr>\r\n                    <td>Операционная система</td>\r\n                    <td>Windows 10</td>\r\n                </tr>\r\n                <tr>\r\n                    <td>Диагональ экрана</td>\r\n                    <td>11.6"</td>\r\n                </tr>\r\n                <tr>\r\n                    <td>Процессор</td>\r\n                    <td>Intel Atom</td>\r\n                </tr>\r\n                <tr>\r\n                    <td>Размер оперативной памяти</td>\r\n                    <td>2 ГБ</td>\r\n                </tr>\r\n                <tr>\r\n                    <td>Жесткий диск</td>\r\n                    <td>(SSD) 32 Гб</td>\r\n                </tr>\r\n            </tbody></table>', 'Заниматься веб-серфингом, просматривать медиаконтент, играть и общаться с помощью ноутбука Irbis NB31 — одно удовольствие. Это качественное и практичное решение, диагональ его экрана составляет 11.6″, глянцевое покрытие добавит краскам сочности и яркости, а с разрешением 1366×768 просмотр станет еще более комфортным. О хорошей графике позаботится встроенная видеокарта Intel HD. За быстродействие и хороший разгон системы даже в условиях многозадачности отвечают 4 ядра процессора, и оперативная память типа DDR3 на 2 ГБ. Причем последнюю можно увеличить посредством карт памяти до 8 ГБ. Если вы еще захотите усилить быстродействие ноутбука, то воспользуйтесь возможностью подключения к нему твердотельных накопителей (SSD), чем объем не должен превышать 32 ГБ. Благодаря 2 портам USB 2.0 можете подключать к Irbis NB31 флеш-карты. С встроенными веб-камерой и микрофоном вы можете использовать устройство для проведения видеоконференций и общения в чатах. 3 ч 30 мин — таково примерное время автономной работы ноутбука благодаря емкости аккумулятора в 10000 мАч.', 11499);

/*!40000 ALTER TABLE products ENABLE KEYS */;

-- 
-- Вывод данных для таблицы reviews
--

/*!40000 ALTER TABLE reviews DISABLE KEYS */;
INSERT INTO reviews VALUES
(1, 'Василий', 'Все круто!', '2020-11-20 22:02:57'),
(3, 'Дмитрий', 'Спасибо!', '2020-11-20 22:03:10');

/*!40000 ALTER TABLE reviews ENABLE KEYS */;

-- 
-- Вывод данных для таблицы users
--

/*!40000 ALTER TABLE users DISABLE KEYS */;
INSERT INTO users VALUES
(1, 'admin', 'a$pG0&32*G!Hs0192023a7bbd73250516f069df18b500a$pG0&32*G!Hs', 'Admin Account', 1),
(2, 'user', 'a$pG0&32*G!Hs6ad14ba9986e3615423dfca256d04e3fa$pG0&32*G!Hs', 'User Account', 0);

/*!40000 ALTER TABLE users ENABLE KEYS */;

-- 
-- Восстановить предыдущий режим SQL (SQL mode)
-- 
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;

-- 
-- Включение внешних ключей
-- 
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;