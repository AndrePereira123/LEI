CREATE DATABASE geshorarios;
SET GLOBAL TIME_ZONE = '+1:00';
CREATE USER IF NOT EXISTS 'me'@'localhost';
SET PASSWORD FOR 'me'@'localhost' = 'mypass';
GRANT ALL PRIVILEGES ON *.* TO 'me'@'localhost';
FLUSH PRIVILEGES;



--DROP DATABASE geshorarios;