#include "CHT.h"

// CHT8305 cht1;
// CHT8305 cht2; 

String readCHTTemperature(CHT8305& cht) {
  float temperature = cht.getTemperature();
  if (isnan(temperature)) {
    return "Failed to read temperature!";
  } else {
    return String(temperature);
  }
}

// Function to read humidity from a DHT sensor
String readCHTHumidity(CHT8305& cht) {
  float humidity = cht.getHumidity();
  if (isnan(humidity)) {
    return "Failed to read humidity!";
  } else {
    return String(humidity);
  }
}