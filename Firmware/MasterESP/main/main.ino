#include "CHT.h"
#include "I2C_Hub.h"
#include "LSM.h"

CHT8305 *chtSensors[] = {&cht1, &cht2};   
LSM6DS3 *lsmSensors[] = {&GyroAccel1, &GyroAccel2};

// Timer for sending data
unsigned long lastTime = 0;
//unsigned long timerDelay = 600000; // 10 minutes
unsigned long timerDelay = 5000; // 5 seconds

void setup() {
  Serial.begin(115200); 
  I2Cinit();
}

void loop() {

  if (millis() - lastTime >= timerDelay) { 
    MP.selectChannel(0);
    Serial.print("AX1 ");
    Serial.print(readLSMAccel(GyroAccel1, 'X'));
    Serial.print("AY1 ");
    Serial.print(readLSMAccel(GyroAccel1, 'Y'));
    Serial.print("AZ1 ");
    Serial.print(readLSMAccel(GyroAccel1, 'Z'));

    MP.selectChannel(1);
    Serial.print("AX2 ");
    Serial.print(readLSMAccel(GyroAccel2, 'X'));
    Serial.print("AY2 ");
    Serial.print(readLSMAccel(GyroAccel2, 'Y'));
    Serial.print("AZ2 ");
    Serial.print(readLSMAccel(GyroAccel2, 'Z'));

    MP.selectChannel(2);
    Serial.print("T1 ");
    Serial.println(readCHTTemperature(cht1));
    Serial.print("T2 ");
    Serial.println(readCHTTemperature(cht2));

    MP.selectChannel(3);
    Serial.print("H1 ");
    Serial.println(readCHTHumidity(cht1));
    Serial.print("H2 ");
    Serial.println(readCHTHumidity(cht2));
  }
}