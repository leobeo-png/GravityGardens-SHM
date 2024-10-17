int rpmBase = 180;
int rpmMax = 2100;
int rpmReq = 0;
int voltageOut = 0;
const int speedpin = 12;
const int rpmPin = 11;
long lastRot = 0;
int readr = false;
float rpm = 0;
float avgRpm = 0;

int mode = 1;

// calibration mode vars
int cmodestart = 0;
int cmodetestStartVal = 60;
int cmodetestcurrent = cmodetestStartVal;
int printinterval = 0;
const int cmodetestlength = 10000;
const int cmodetestinterval = 10;
const int cmodetestmaxspeed = 180;
int calibratedBaseSpeed = 0;
int calibratedMaxSpeedAtVoltage = 0;
int calibratedMaxSpeedVoltage = 50;

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
  // Set mode
  

  // Read rpm
  float crpm = readRPM(rpmPin);
  if(crpm != -1) {
    avgRpm = 0.3f * avgRpm + 0.7f * crpm;
    Serial.print("rpm: ");
    Serial.println(avgRpm);
  }
  

  if(Serial.available() > 1 && mode == 0) {
    String inputstring = Serial.readString();
    Serial.println(inputstring);
    if(inputstring.charAt(0) >= '0' && inputstring.charAt(0) <= '9') {
      // Set req rpm
      int v = atoi(inputstring.c_str());
      // int v = Serial.parseInt();
      Serial.print("Set to: ");
      Serial.print(v);
      Serial.print(" rpm: ");
      rpmReq = v;
    } else if(inputstring[0] == 'c') {
      // Calibrate
      Serial.println("Calibrate mode");
      mode = 1;
      cmodetestcurrent = cmodetestStartVal;
      cmodestart = millis();
    }
  }

  // Default
  if(mode == 0) {
    // voltageOut = 255.0f * ((float)rpmReq - (float)rpmBase) / (float)(rpmMax - rpmBase);
    voltageOut = calibratedMaxSpeedVoltage * ((float)rpmReq - (float)calibratedBaseSpeed) / (float)(calibratedMaxSpeedAtVoltage - calibratedBaseSpeed);
    if(voltageOut < 0) voltageOut = 0;
    if(voltageOut > 255) voltageOut = 255;
    //Serial.print("vout: ");
    // Serial.println(voltageOut);
    analogWrite(speedpin, voltageOut);
  } else if(mode == 1) {
    int currentCalibrationStep = floor((millis() - cmodestart) / cmodetestlength);150
    int currentOut = currentCalibrationStep * cmodetestinterval;

    if(currentCalibrationStep == 0) {
      calibratedBaseSpeed = floor(avgRpm);
    } else if(avgRpm >= cmodetestmaxspeed) {
      calibratedMaxSpeedVoltage = currentCalibrationStep * cmodetestinterval;
      calibratedMaxSpeedAtVoltage = floor(avgRpm);
      mode = 0;
      Serial.print("Set max volt% to ");
      Serial.print(calibratedMaxSpeedVoltage);
      Serial.print(" at rpm ");
      Serial.println(calibratedMaxSpeedAtVoltage);
    }
    if(printinterval % 100000 == 0) {
      Serial.print("coutput: ");
      Serial.println(currentOut);
      Serial.println(avgRpm);
    }
    analogWrite(speedpin, currentOut);
    printinterval++;

  }
}