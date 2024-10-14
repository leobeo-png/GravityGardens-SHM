#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>  // Needed for WebSocket functionality

// Replace with your network credentials
const char* ssid = "GG-SHM";
const char* password = "GravityGardens!";

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Function to handle WebSocket events
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("New WebSocket client connected");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("WebSocket client disconnected");
  } else if (type == WS_EVT_DATA) {
    Serial.print("Data received: ");
    for (size_t i = 0; i < len; i++) {
      Serial.print((char) data[i]);  // Print received data
    }
    Serial.println();
  }
}

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print the ESP32 IP address
  Serial.println(WiFi.localIP());

  // Initialize WebSocket server and define event handler
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  // Start the server
  server.begin();
}

void loop() {
  // Clean up clients
  ws.cleanupClients();
}
