#include "DHT22.h"
#include "config.h"
#include "CHT.h"
#include "I2C_Hub.h"
#include "LSM.h"

DHT *dhtSensors[] = {&dht1, &dht2, &dht3, &dht4};  // Array for DHT sensors
CHT8305 *chtSensors[] = {&cht1, &cht2};   

// Timer for sending data
unsigned long lastTime = 0;
//unsigned long timerDelay = 600000; // 10 minutes
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200); 
  DHTinit();
  I2Cinit();
  // Set minimum security for the raspberry pi
  WiFi.setMinSecurity(WIFI_AUTH_WPA_PSK);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println(WiFi.localIP());
}

void loop() {
  // Millis for send time
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){ // Check if it is connected
      for (int i = 0; i < 4; i++) {
        sendSensorData(tempUrls[i], readDHTTemperature(*dhtSensors[i]));
        sendSensorData(humUrls[i], readDHTHumidity(*dhtSensors[i]));
      }

      // Send temperature data from CHT sensors
      for (int i = 0; i < 2; i++) {
        sendSensorData(tempUrls[i + 4], readCHTTemperature(*chtSensors[i]));
        sendSensorData(humUrls[i + 4], readCHTHumidity(*chtSensors[i]));
      }
    }
    lastTime = millis();
  }
}
