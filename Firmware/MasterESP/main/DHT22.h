#ifndef DHT22_H
#define DHT22_H

#include <DHT.h>

// DHT Pins
#define DHTPIN1 27  
#define DHTPIN2 26  
#define DHTPIN3 25
#define DHTPIN4 24  

#define DHTTYPE  DHT22     // DHT 22 (AM2302)

// Initialize DHT sensors
extern DHT dht1;
extern DHT dht2;
extern DHT dht3;
extern DHT dht4;

void DHTinit();
String readDHTTemperature(DHT& dht);
String readDHTHumidity(DHT& dht);

#endif