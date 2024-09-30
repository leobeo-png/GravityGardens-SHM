-- GravityGardens DB setup
DROP USER IF EXISTS 'gguser';
CREATE USER 'gguser'@'localhost' IDENTIFIED BY 'GravityGardens!';

DROP DATABASE IF EXISTS ggdata;
CREATE DATABASE ggdata;
GRANT CREATE, ALTER, DROP, INSERT, UPDATE, DELETE, SELECT, REFERENCES ON ggdata.* TO 'gguser'@'localhost';
FLUSH PRIVILEGES;

USING ggdata;

DROP TABLE IF EXISTS experiments;
CREATE TABLE experiments (
	id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
	sensornumber INT NOT NULL,
);

INSERT INTO experiments (sensornumber) VALUES (1);
INSERT INTO experiments (sensornumber) VALUES (2);
INSERT INTO experiments (sensornumber) VALUES (3);
INSERT INTO experiments (sensornumber) VALUES (4);
INSERT INTO experiments (sensornumber) VALUES (5);
INSERT INTO experiments (sensornumber) VALUES (6);

-- Database table creation
DROP TABLE IF EXISTS experimentLogs;
CREATE TABLE experimentLogs (
	id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
	recordtime TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
	temperature FLOAT,
	humidity FLOAT,
	rpm FLOAT,
	lightstatus BOOLEAN,
	lastwatered DATETIME
);

DROP TABLE IF EXISTS wateringLogs;
CREATE TABLE wateringLogs (
	id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
	FOREIGN KEY (experimentid) REFERENCES experiments(id),
	wateringtime DATETIME
);

DROP TABLE IF EXISTS sensorLogs;
CREATE TABLE sensorLogs (
	id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
	FOREIGN KEY (experimentid) REFERENCES experimentLogs(id),
	temperature FLOAT,
	humidity FLOAT
);

DROP TABLE IF EXISTS speedsensorLogs;
CREATE TABLE speedsensorLogs (
	id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
	sensornumber INT NOT NULL,
	recordtime TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
	axis_x FLOAT,
	axis_y FLOAT,
	axis_z FLOAT,
	gyro_x FLOAT,
	gyro_y FLOAT,
	gyro_z FLOAT
);

DROP TABLE IF EXISTS experimentsettings;
CREATE TABLE settings (
	id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
	settingname VARCHAR(26) NOT NULL,
	settingdata VARCHAR(26) NOT NULL
);

INSERT INTO experimentsettings (settingname, settingdata) VALUES ("lights_cycle_time", "60s");
INSERT INTO experimentsettings (settingname, settingdata) VALUES ("lights_on_time", "30s");
INSERT INTO experimentsettings (settingname, settingdata) VALUES ("target_rpm", "1000");
INSERT INTO experimentsettings (settingname, settingdata) VALUES ("rpm_zero", "180");
INSERT INTO experimentsettings (settingname, settingdata) VALUES ("rpm_max", "2100");

