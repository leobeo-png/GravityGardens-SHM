const int speedpin = 5;
const int rpmPin = 16;
int speed = 90;

int doAutoAdjust = 1;
long lastAutoAdjust = 0;
long autoAdjustSpace = 1000;
long autoAdjustStart = 0;

int setRpm = 120;
int setRpmBuffer = 3;
int setRpmChangeVoltage = 1;

long lastRot = 0;
float rpm = 0;
float avgRpm = 0;
int readr = 0;
float readRPM(int rPin) {
//  Serial.println(readr);
  if(digitalRead(rPin) == LOW) {
    readr++;

    if(readr == 100) {
      long current = micros();
      long diff = current - lastRot;
      lastRot = current;
      int rpmtmp = (60 * 1000000) / (float)diff;
      if(rpmtmp < 1000) {
        Serial.println(rpmtmp);
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
	Serial.begin(115200);
  
  analogWrite(speedpin, speed);
}

void loop() {
  float crpm = readRPM(rpmPin);
  if(crpm != -1) {
    avgRpm = 0.3f * avgRpm + 0.7f * crpm;
    Serial.print("SP ");
    Serial.println(avgRpm);
  }

	if(Serial.available() > 2) {
		int v = Serial.parseInt();
    Serial.print("Set speed to ");
    Serial.println(v);
    setRpm = v;
//    analogWrite(speedpin, speed/);
	}

  long cMillis = millis();
  if(doAutoAdjust && cMillis > lastAutoAdjust + autoAdjustSpace && cMillis > autoAdjustStart) {
    lastAutoAdjust = cMillis;

    if(avgRpm - setRpmBuffer > setRpm) {
      speed -= setRpmChangeVoltage;
      Serial.print("Adjusted to ");
      Serial.println(speed);
      analogWrite(speedpin, speed);
    }
    if(avgRpm + setRpmBuffer < setRpm) {
      speed += setRpmChangeVoltage;
      Serial.print("Adjusted to ");
      Serial.println(speed);
      analogWrite(speedpin, speed);
    }
  }
}
