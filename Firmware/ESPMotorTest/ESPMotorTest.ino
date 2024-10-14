const int speedpin = 19;
int speed = 0;

void setup() {
	pinMode(speedpin, OUTPUT);
	Serial.begin(9600);
}

void loop() {
	if(Serial.available() > 1) {
		int v = Serial.parseInt();
		Serial.print("Set speed to ");
		Serial.println(v);
		analogWrite(speedpin, v);
	}
}