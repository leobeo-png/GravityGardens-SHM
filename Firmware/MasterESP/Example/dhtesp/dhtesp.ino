#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "LittleFS.h"
#include "TCA9548.h"
#include "LSM6DS3.h"
#include "Wire.h"


// Replace with the network credentials (make sure the PC is also on the same network)
const char* ssid = "Pixel_5658";
const char* password = "leo07072005";

// DHT Pins
#define DHTPIN1 27  
#define DHTPIN2 26  
#define DHTPIN3 25  


#define DHTTYPE    DHT22     // DHT 22 (AM2302)

// Initialize DHT sensors
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);
LSM6DS3 GyroAccel1;
LSM6DS3 GyroAccel2;

TCA9548 MP(0x70);

// AsyncWebServer Port 80
AsyncWebServer server(80);

String readDHTTemperature(DHT& dht) {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    return "--";
  } else {
    return String(t);
  }
}

String readDHTHumidity(DHT& dht) {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read humidity from DHT sensor!");
    return "--";
  } else {
    return String(h);
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
    Serial.print("Accel ");
    Serial.print(axis);
    Serial.print(": ");
    Serial.println(accelValue);
    return String(accelValue);
  }
}
  
// Initialize LittleFS
void initLittleFS() {
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  else{
    Serial.println("LittleFS mounted successfully");
  }
}


void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize the TCA9548 multiplexer
  if (!MP.begin()) {
    Serial.println("Failed to initialize TCA9548 multiplexer!");
    while (1);
  }
  Serial.println("TCA9548 multiplexer initialized!");

  // Test sensor 1 (Channel 0)
  MP.selectChannel(0);
  if (GyroAccel1.begin() == false) {
    Serial.println("Failed to initialize LSM6DS3 sensor 1 on Channel 0!");
  } else {
    Serial.println("LSM6DS3 sensor 1 initialized on Channel 0!");
  }

  // Test sensor 2 (Channel 1)
  MP.selectChannel(1);
  if (GyroAccel2.begin() == false) {
    Serial.println("Failed to initialize LSM6DS3 sensor 2 on Channel 1!");
  } else {
    Serial.println("LSM6DS3 sensor 2 initialized on Channel 1!");
  }

  Serial.println("Initialization complete.\n");
  initLittleFS();
  dht1.begin();
  dht2.begin();
  dht3.begin();

  WiFi.setMinSecurity(WIFI_AUTH_WPA_PSK);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println(WiFi.localIP());

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });

  // Route to load style.css file

  server.serveStatic("/", LittleFS, "/");

// Serve temperature and humidity data for each sensor as plain text
  server.on("/temperature1", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readDHTTemperature(dht1));
  });
  
  server.on("/temperature2", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readDHTTemperature(dht2));
  });

  server.on("/temperature3", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readDHTTemperature(dht3));
  });

  server.on("/humidity1", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readDHTHumidity(dht1));
  });
  
  server.on("/humidity2", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readDHTHumidity(dht2));
  });

  server.on("/humidity3", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readDHTHumidity(dht3));
  });

  server.on("/accel1", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readLSMAccel(GyroAccel1, 'X'));
  });
  
  server.on("/accel2", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readLSMAccel(GyroAccel1, 'Y'));
  });

  server.on("/accel3", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readLSMAccel(GyroAccel1, 'Z'));
  });

  server.on("/accel4", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readLSMAccel(GyroAccel2, 'X'));
  });
  
  server.on("/accel5", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readLSMAccel(GyroAccel2, 'Y'));
  });

  server.on("/accel6", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readLSMAccel(GyroAccel2, 'Z'));
  });

  server.begin();
}


void loop() {
}