#ifndef LSM6DS3_H
#define LSM6DS3_H

#include <Arduino.h>
#include "LSM6DS3.h"

// extern LSM6DS3 GyroAccel1;
// extern LSM6DS3 GyroAccel2;

String readLSMGyro(LSM6DS3& GyroAccel, char axis);
String readLSMAccel(LSM6DS3& GyroAccel, char axis);

#endif
