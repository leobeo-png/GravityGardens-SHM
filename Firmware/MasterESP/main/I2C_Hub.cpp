#include "I2C_Hub.h"
#include "CHT.h"
#include "LSM.h"

//TCA9548 MP(0x70);

void I2Cinit() {
  Wire.begin();
  //  initialize multiplexer
  if (MP.begin() == false)
  {
    Serial.println("Multiplexer error");
  }
  // initialize the i2c sensors
  MP.enableChannel(2);
  Serial.println(MP.isEnabled(2) ? "Channel 2 enabled" : "Channel 2 not enabled");
  MP.selectChannel(2);
  if (MP.isConnected(0x6A) == false){
    Serial.println("gyro1 not connected");
  } 
  if (GyroAccel1.begin() == false)
  {
    Serial.println("gyro1 error");
  }
  MP.enableChannel(5);
  Serial.println(MP.isEnabled(5) ? "Channel 5 enabled" : "Channel 5 not enabled");
  MP.selectChannel(5);
  if (MP.isConnected(0x6A) == false){
    Serial.println("gyro2 not connected");
  } 
  if (GyroAccel2.begin() == false)
  {
    Serial.println("gyro2 error");
  }
  MP.enableChannel(7);
  Serial.println(MP.isEnabled(7) ? "Channel 7 enabled" : "Channel 7 not enabled");
  MP.selectChannel(7);
  if (MP.isConnected(0x40) == false){
    Serial.println("temp1 not connected");
  } 
  if (cht1.begin() == false )
  {
    Serial.println("temp1 error");
  }
  MP.enableChannel(1);
  Serial.println(MP.isEnabled(1) ? "Channel 1 enabled" : "Channel 1 not enabled");
  MP.selectChannel(1);
  if (MP.isConnected(0x40) == false){
    Serial.println("temp2  not connected");
  } 
  if (cht2.begin() == false )
  {
    Serial.println("temp2 error");
  }
}