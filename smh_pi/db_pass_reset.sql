-- Clean install
UPDATE mysql.user SET Password = PASSWORD('GravityGardens!') WHERE User = 'root';
DELETE FROM mysql.user WHERE User = '';
DELETE FROM mysql.user WHERE User = 'root' AND Host NOT IN ('localhost', '127.0.0.1');
DROP DATABASE IF EXISTS test;
FLUSH PRIVILEGES;

-- GravityGardens DB setup
DROP USER IF EXISTS 'gguser';
CREATE USER 'gguser'@'localhost' IDENTIFIED BY 'GravityGardens!';

DROP DATABASE IF EXISTS ggdata;
CREATE DATABASE ggdata;
GRANT CREATE, ALTER, DROP, INSERT, UPDATE, DELETE, SELECT, REFERENCES ON ggdata.* TO 'gguser'@'localhost';
FLUSH PRIVILEGES;
