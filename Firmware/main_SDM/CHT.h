#ifndef CHT8305_H
#define CHT8305_H

#include <Arduino.h>  // Include Arduino.h for String and other definitions
#include "CHT8305.h"

// External declarations for CHT8305 sensor objects
// extern CHT8305 cht1;
// extern CHT8305 cht2; 

// Function declarations
String readCHTTemperature(CHT8305& cht);
String readCHTHumidity(CHT8305& cht);

#endif // CHT8305_H
