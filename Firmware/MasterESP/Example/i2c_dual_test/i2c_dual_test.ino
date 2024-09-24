#include "LSM6DS3.h"
#include "Wire.h"

LSM6DS3 myIMU(I2C_MODE, 0x6A);
LSM6DS3 myIMU2(I2C_MODE, 0x6A); // Initialize this on the second I2C bus

volatile double lastReadTime = 0;  // Store the last read time
volatile double currentReadTime = 0; // Store the current read time
volatile double readInterval = 0;   // Time interval between sensor readings
unsigned long printInterval = 1000; // Time interval for printing in milliseconds
unsigned long lastPrintTime = 0;     // Last time data was printed

void setup() {
    // Initialize Serial for debugging
    Serial.begin(115200);
    // Initialize both I2C buses
    Wire.begin(21, 22);  // First I2C bus
    Wire1.begin(19, 18); // Second I2C bus for the second sensor

    // Initialize first IMU
    if (myIMU.begin() != 0) {
        Serial.println("Device 1 error");
    } else {
        Serial.println("Device 1 OK!");
    }

    // Initialize second IMU
    if (myIMU2.begin() != 0) {
        Serial.println("Device 2 error");
    } else {
        Serial.println("Device 2 OK!");
    }
}

void loop() { 
    currentReadTime = millis();  // Get the current time in milliseconds

    // Read from the first IMU
    float accelX1 = myIMU.readFloatAccelX();
    float accelY1 = myIMU.readFloatAccelY();
    float accelZ1 = myIMU.readFloatAccelZ();
    
    float gyroX1 = myIMU.readFloatGyroX();
    float gyroY1 = myIMU.readFloatGyroY();
    float gyroZ1 = myIMU.readFloatGyroZ();

    // Read from the second IMU
    float accelX2 = myIMU2.readFloatAccelX();
    float accelY2 = myIMU2.readFloatAccelY();
    float accelZ2 = myIMU2.readFloatAccelZ();
    
    float gyroX2 = myIMU2.readFloatGyroX();
    float gyroY2 = myIMU2.readFloatGyroY();
    float gyroZ2 = myIMU2.readFloatGyroZ();
  
    // Calculate the time interval between the current reading and the last reading
    if (lastReadTime != 0) {  // Make sure it’s not the first read
        readInterval = currentReadTime - lastReadTime;
    }

    // Update the last read time
    lastReadTime = currentReadTime;

    // Print data if enough time has passed
    if (currentReadTime - lastPrintTime >= printInterval) {
        lastPrintTime = currentReadTime; // Update last print time
        Serial.print("Read Interval: ");
        Serial.print(readInterval); Serial.print(" ms\t");

        Serial.print("Accel: X1 = "); Serial.print(accelX1, 4); 
        Serial.print(", Y1 = "); Serial.print(accelY1, 4); 
        Serial.print(", Z1 = "); Serial.print(accelZ1, 4); 
        Serial.print(" | Accel: X2 = "); Serial.print(accelX2, 4); 
        Serial.print(", Y2 = "); Serial.print(accelY2, 4); 
        Serial.print(", Z2 = "); Serial.print(accelZ2, 4); 
        Serial.print(" | Gyro: X1 = "); Serial.print(gyroX1, 4); 
        Serial.print(", Y1 = "); Serial.print(gyroY1, 4); 
        Serial.print(", Z1 = "); Serial.print(gyroZ1, 4); 
        Serial.print(" | Gyro: X2 = "); Serial.print(gyroX2, 4); 
        Serial.print(", Y2 = "); Serial.print(gyroY2, 4); 
        Serial.print(", Z2 = "); Serial.println(gyroZ2, 4);
    }
}
