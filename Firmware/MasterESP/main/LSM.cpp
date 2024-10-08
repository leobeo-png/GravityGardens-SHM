#include "LSM.h"

LSM6DS3 GyroAccel1;
LSM6DS3 GyroAccel2;

String readLSMGyro(LSM6DS3& GyroAccel, char axis) {
  float gyroValue;

  switch (axis) {
    case 'X':
      gyroValue = GyroAccel.readFloatGyroX();
      break;
    case 'Y':
      gyroValue = GyroAccel.readFloatGyroY();
      break;
    case 'Z':
      gyroValue = GyroAccel.readFloatGyroZ();
      break;
    default:
      return "Invalid axis!";
  }

  // Check for invalid readings
  if (isnan(gyroValue)) {
    return "Failed to read gyroscope " + String(axis) + " axis!";
  } else {
    return String(gyroValue);
  }
}

String readLSMAccel(LSM6DS3& GyroAccel, char axis) {
  float accelValue;

  switch (axis) {
    case 'X':
      accelValue = GyroAccel.readFloatAccelX();
      break;
    case 'Y':
      accelValue = GyroAccel.readFloatAccelY();
      break;
    case 'Z':
      accelValue = GyroAccel.readFloatAccelZ();
      break;
    default:
      return "Invalid axis!";
  }

  // Check for invalid readings
  if (isnan(accelValue)) {
    return "Failed to read gyroscope " + String(axis) + " axis!";
  } else {
    return String(accelValue);
  }
}
