// const express = require("express");
// const app = express();
// const port = 8000;

// app.use(express.static("static"));

// app.get("/", (req, res) => {
// 	res.send("Test");
// });
// app.post("/data/{experminet_id}/humidty");

// app.listen(port, () => {
// 	console.log(`Running on http://localhost:${port}`);
// });






















const WebSocket = require('ws');

// Replace <ESP32_IP> with the IP address of your ESP32
const ws = new WebSocket('ws://<ESP32_IP>/ws');

ws.on('open', function open() {
  console.log('Connected to ESP32 WebSocket server');
  
  // Send data to the ESP32
  ws.send('Hello ESP32, this is Raspberry Pi!');
});

ws.on('message', function incoming(data) {
  console.log(`Received message from ESP32: ${data}`);
});

ws.on('close', function close() {
  console.log('Disconnected from WebSocket server');
});

ws.on('error', function error(err) {
  console.error('WebSocket error: ', err);
});
