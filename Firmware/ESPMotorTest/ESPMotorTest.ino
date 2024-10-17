const int speedpin = 12;
const int rpmPin = 11;
int speed = 0;


long lastRot = 0;
float rpm = 0;
float avgRpm = 0;
int readr = 0;
float readRPM(int rPin) {
  if(digitalRead(rPin) == LOW) {
    readr++;

    if(readr == 100) {
      long current = micros();
      long diff = current - lastRot;
      lastRot = current;
      int rpmtmp = (60 * 1000000) / (float)diff;
      if(rpmtmp < 1000) {
        //Serial.println(rpmtmp);
        rpm = rpmtmp;
        return rpm;
      }
    }
  } else if(digitalRead(rPin) == HIGH) {
    readr--;
    if (readr < 0) readr = 0;
  }
  return -1;
}

void setup() {
	pinMode(speedpin, OUTPUT);
	pinMode(rpmPin, INPUT);
	Serial.begin(9600);
}

void loop() {
  float crpm = readRPM(rpmPin);
  if(crpm != -1) {
    avgRpm = 0.3f * avgRpm + 0.7f * crpm;
    Serial.print("rpm: ");
    Serial.println(avgRpm);
  }

	if(Serial.available() > 2) {
		int v = Serial.parseInt();
    Serial.print("Set speed to ");
    Serial.println(v);
    analogWrite(speedpin, v);
	}
}