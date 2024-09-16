#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

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

// HTML content to display on the web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Sensor Dashboard</title>
  <style>
    body, html {
      margin: 0;
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      color: #333;
      background-color: #f4f4f9;
      text-align: center;
    }
    header {
      background-color: #2c3e50;
      color: #ecf0f1;
      padding: 1rem;
      position: fixed;
      width: 100%;
      top: 0;
      left: 0;
      box-shadow: 0 2px 5px rgba(0,0,0,0.2);
    }
    .container {
      margin-top: 120px; /* Space for fixed header */
      display: grid;
      grid-template-columns: repeat(2, 1fr); /* 2 columns */
      gap: 20px; /* Space between cards */
      justify-content: center;
      padding: 1rem;
    }
    .card {
      background-color: #fff;
      padding: 20px;
      border-radius: 12px;
      width: 250px;
      box-shadow: 0 4px 8px rgba(0,0,0,0.1);
      transition: transform 0.3s, box-shadow 0.3s;
    }
    .card:hover {
      transform: scale(1.05);
      box-shadow: 0 6px 12px rgba(0,0,0,0.15);
    }
    .temperature { color: #e67e22; }
    .humidity { color: #3498db; }
    .reading { font-size: 2rem; }
    h3 {
      margin: 0;
      font-size: 1.2rem;
    }
  </style>
</head>
<body>

  <header>
    <h1>ESP32 Sensor Dashboard</h1>
  </header>

  <div class="container">
    <!-- Sensor 1 -->
    <div class="card temperature">
      <h3>Sensor 1 - Temperature</h3>
      <p class="reading" id="temperature1">--</p>
    </div>
    <div class="card humidity">
      <h3>Sensor 1 - Humidity</h3>
      <p class="reading" id="humidity1">--</p>
    </div>

    <!-- Sensor 2 -->
    <div class="card temperature">
      <h3>Sensor 2 - Temperature</h3>
      <p class="reading" id="temperature2">--</p>
    </div>
    <div class="card humidity">
      <h3>Sensor 2 - Humidity</h3>
      <p class="reading" id="humidity2">--</p>
    </div>

    <!-- Sensor 3 -->
    <div class="card temperature">
      <h3>Sensor 3 - Temperature</h3>
      <p class="reading" id="temperature3">--</p>
    </div>
    <div class="card humidity">
      <h3>Sensor 3 - Humidity</h3>
      <p class="reading" id="humidity3">--</p>
    </div>
  </div>

  <script>
    function fetchData(sensorId) {
      // Fetch temperature for the sensor
      setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("temperature" + sensorId).innerHTML = this.responseText + " °C";
          }
        };
        xhttp.open("GET", "/temperature" + sensorId, true);
        xhttp.send();
      }, 4000); // 4 seconds interval

      // Fetch humidity for the sensor
      setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("humidity" + sensorId).innerHTML = this.responseText + " %";
          }
        };
        xhttp.open("GET", "/humidity" + sensorId, true);
        xhttp.send();
      }, 4000); // 4 seconds interval
    }

    // Call fetchData for all three sensors
    fetchData(1);  // Sensor 1
    fetchData(2);  // Sensor 2
    fetchData(3);  // Sensor 3

  </script>

</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  dht1.begin();
  dht2.begin();
  dht3.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println(WiFi.localIP());

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

  // load the HTML web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.begin();
}

void loop() {
  delay(1000); // need some sort of delay for the CPU, if not then overtime it will crash and "Panic"
}