<<<<<<< HEAD
#include "DHT22.h"
#include "config.h"
=======
>>>>>>> origin/SerialMain
#include "CHT.h"
#include "I2C_Hub.h"
#include "LSM.h"

<<<<<<< HEAD
DHT *dhtSensors[] = {&dht1, &dht2, &dht3, &dht4};  // Array for DHT sensors
CHT8305 *chtSensors[] = {&cht1, &cht2};   
=======
CHT8305 *chtSensors[] = {&cht1, &cht2};   
LSM6DS3 *lsmSensors[] = {&GyroAccel1, &GyroAccel2};
>>>>>>> origin/SerialMain

// Timer for sending data
unsigned long lastTime = 0;
//unsigned long timerDelay = 600000; // 10 minutes
<<<<<<< HEAD
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
=======
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
>>>>>>> origin/SerialMain
  }
}
