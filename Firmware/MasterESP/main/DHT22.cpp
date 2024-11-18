#include "DHT22.h"

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);
DHT dht4(DHTPIN4, DHTTYPE);
// Initialize the DHT sensors
void DHTinit() {
  dht1.begin();
  dht2.begin();
  dht3.begin();
}

// Function to read temperature from a DHT sensor
String readDHTTemperature(DHT& dht) {
  float temperature = dht.readTemperature();
  if (isnan(temperature)) {
    return "Failed to read temperature!";
  } else {
    return String(temperature);
  }
}

// Function to read humidity from a DHT sensor
String readDHTHumidity(DHT& dht) {
  float humidity = dht.readHumidity();
  if (isnan(humidity)) {
    return "Failed to read humidity!";
  } else {
    return String(humidity);
  }
}
