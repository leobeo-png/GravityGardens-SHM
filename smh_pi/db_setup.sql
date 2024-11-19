-- GravityGardens DB setup
DROP USER IF EXISTS gguser;
FLUSH PRIVILEGES;
CREATE OR REPLACE USER gguser@"127.0.0.1" IDENTIFIED BY 'GravityGardens!';
FLUSH PRIVILEGES;

DROP DATABASE IF EXISTS ggdata;
CREATE DATABASE ggdata;
GRANT CREATE, ALTER, DROP, INSERT, UPDATE, DELETE, SELECT, REFERENCES ON ggdata.* TO 'gguser'@'localhost';
FLUSH PRIVILEGES;

USE ggdata;

DROP TABLE IF EXISTS experiments;
CREATE TABLE experiments (
	id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
	sensornumber INT NOT NULL
);

INSERT INTO experiments (sensornumber) VALUES (1), (2), (3), (4), (5), (6);

-- Database table creation
DROP TABLE IF EXISTS experimentLogs;
CREATE TABLE experimentLogs (
	id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
	recordtime TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
	rpm FLOAT,
	lightstatus BOOLEAN
);

DROP TABLE IF EXISTS wateringLogs;
CREATE TABLE wateringLogs (
	id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
	experimentid INT NOT NULL, 
	wateringtime DATETIME,
	CONSTRAINT fk_experiment FOREIGN KEY (experimentid) REFERENCES experiments(id)
);

DROP TABLE IF EXISTS sensorLogs;
CREATE TABLE sensorLogs (
	id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
	experimentlogid INT NOT NULL,
	temperature FLOAT,
	humidity FLOAT,
	CONSTRAINT fk_experimentlog FOREIGN KEY (experimentlogid) REFERENCES experimentLogs(id)
);

DROP TABLE IF EXISTS speedsensorLogs;
-- CREATE TABLE speedsensorLogs (
--	id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
--	sensornumber INT NOT NULL,
--	recordtime TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
--	axis_x FLOAT,
--	axis_y FLOAT,
--	axis_z FLOAT,
--	gyro_x FLOAT,
--	gyro_y FLOAT,
--	gyro_z FLOAT
-- );

DROP TABLE IF EXISTS experimentsettings;
CREATE TABLE experimentsettings (
	id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
	settingname VARCHAR(26) NOT NULL,
	settingdata VARCHAR(26) NOT NULL
);

INSERT INTO experimentsettings (settingname, settingdata) VALUES 
	("lights_cycle_time", "60s"), 
	("lights_on_time", "30s"), 
	("target_rpm", "1000"), 
	("rpm_zero", "180"), 
	("rpm_max", "2100");

