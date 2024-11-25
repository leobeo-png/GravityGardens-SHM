# GravityGardens-SHM
This repository is for the Project Gravity Gardens Small Hypergravity Machine. 
# Project Goals
- To create a control system that accommodates different sensors and handles incoming data
- To have and interface that shows the Temperature, Humidity, Motor Acceleration, and RPM
- To have a log of all the data for the scientist to analyze it
# System Architecture
Our initial idea is to have a webserver running on the ESP32, connected to a Raspberry Pi database to save the logs. The sensors would be placed on the spinning motor with another ESP32 running ESP-NOW to send back to the webserver, as we initially thought it needed to be spinning with the machine. 
Although, this idea fell through with some complications such as Wi-Fi connection being unstable and the sensors are not needed within the motor contraption. 

The current implementation involves an ESP32 with the sensors outside the contraption. It will communicate with the Raspberry Pi with a USB cable using Serial. The Raspberry Pi will handle the webserver, database and sending commands to the ESP32. 



# Components
- CHT8305 Temperature and Humidity Sensor I2C
- Seeed Arduino LSM6DS3 I2C x2
- TCA9548 I2C Multiplexer
- ESP32 DEVKITC v4
- Raspberry Pi 4B 1GB
- IR Sensor for Reading the RPM
- Extra Laptop for the website
- E-Stop button
# Software
All the coding of the ESP32 is with the Arduino IDE. This is due to the extensive libraries, ease of use and support for all the sensors and microcontroller. 

The Raspberry Pi is using MariaDB and Express.js to interface with the website and database.
# How to run the server
TBA
# How to setup the ESP32 for Arduino IDE
Refer to this [guide](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/).


