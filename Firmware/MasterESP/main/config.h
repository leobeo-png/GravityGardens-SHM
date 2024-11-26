#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <WiFi.h>
#include <HTTPClient.h>  // Ensure HTTPClient is included

// Declare WiFi credentials as extern, they will be defined in the .cpp file
extern const char* ssid;
extern const char* password;
extern const char* tempUrls[];
extern const char* humUrls[];

// Declare serverName as extern, will be defined in the .cpp file
extern String serverName;

// Function prototype for sending sensor data
void sendSensorData(String url, String data);

#endif
