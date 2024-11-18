#include "config.h"

// Define WiFi credentials
const char* ssid = "GG-SHM";
const char* password = "GravityGardens!";

// Define the server URL
String serverName = "http://localhost:8000/";

const char* tempUrls[] = {
  "https://localhost:8000/temperature1", "https://localhost:8000/temperature2",
  "https://localhost:8000/temperature3", "https://localhost:8000/temperature4",
  "https://localhost:8000/temperature5", "https://localhost:8000/temperature6"
};
const char* humUrls[] = {
  "https://localhost:8000/humidity1", "https://localhost:8000/humidity2",
  "https://localhost:8000/humidity3", "https://localhost:8000/humidity4",
  "https://localhost:8000/humidity5", "https://localhost:8000/humidity6"
};

void sendSensorData(String url, String data) {
  if (WiFi.status() == WL_CONNECTED) {  // Check WiFi connection status
    HTTPClient http;

    http.begin(url);  // Specify the URL
    http.addHeader("Content-Type", "text/plain");

    int httpResponseCode = http.POST(data);  // Send data as POST request

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);  // HTTP response code
      Serial.println(response);          // Server response
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Close connection
  } else {
    Serial.println("Error: Not connected to WiFi");
  }
}
