#include "I2C_Hub.h"
#include "CHT.h"
#include "LSM.h"

PCA9546 MP(0x70);

void I2Cinit() {
  Wire.begin();
  //  initialize multiplexer
  if (MP.begin() == false)
  {
    Serial.println("Multiplexer error");
  }
  // initialize the i2c sensors
  MP.selectChannel(0);
  if (GyroAccel1.begin() == false)
  {
    Serial.println("gyro error");
  }
  MP.selectChannel(1);
  if (GyroAccel2.begin() == false)
  {
    Serial.println("gyro error");
  }
  MP.selectChannel(2);
  if (cht1.begin() == false )
  {
    Serial.println("temp1 error");
  }
  MP.selectChannel(3);
  if (cht2.begin() == false )
  {
    Serial.println("temp2 error");
  }
}