#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "LittleFS.h"

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

// String getSensorDataCSV() {
//   // Read data from each sensor
//   String t1 = readDHTTemperature(dht1);
//   String h1 = readDHTHumidity(dht1);
//   String t2 = readDHTTemperature(dht2);
//   String h2 = readDHTHumidity(dht2);
//   String t3 = readDHTTemperature(dht3);
//   String h3 = readDHTHumidity(dht3);

//   // Format: t1,h1,t2,h2,t3,h3
//   String data = t1 + "," + h1 + "," + t2 + "," + h2 + "," + t3 + "," + h3;

//   return data;
// }

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
  initLittleFS();
  dht1.begin();
  dht2.begin();
  dht3.begin();

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


  server.begin();
}

void loop() {
  delay(1000); // need some sort of delay for the CPU, if not then overtime it will crash and "Panic"
}