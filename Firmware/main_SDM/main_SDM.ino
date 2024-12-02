#include "config.h"
#include "CHT.h"
#include "I2C_Hub.h"
#include "LSM.h"
#include "string.h"
#include "wire.h"
#include "TCA9548.h"

CHT8305 *chtSensors[] = {&cht1, &cht2};   
LSM6DS3 *lsmSensors[] = {&GyroAccel1, &GyroAccel2};


//pinout
const int speedpin = 5;
const int rpmPin = 14;
const int doorPin = 17;
const int EstopPin = 12;

//global vars
int accelTrheshold = 100;
//doorvars
int doorEnabled = false;
int doorIsOpen = false;
//Motor vars
int speed = 100;
int doAutoAdjust = 1;
long lastAutoAdjust = 0;
long autoAdjustSpace = 5000;
long autoAdjustStart = 30000;
long lastReadRpm = 0;
long counter = 0;
long lastCount = 0;
//Accel vars
long lastAccelMillies = 0;
long accelMillis;

int setRpm = 155;
int setRpmBuffer = 3;
int setRpmChangeVoltage = 1;

long lastRot = 0;
float rpm = 0;
float avgRpm = 0;
int readr = 0;

void setup() {
  //pin setup
  pinMode(speedpin, OUTPUT);
	pinMode(rpmPin, INPUT);
  pinMode(doorPin, INPUT);
  pinMode(EstopPin, INPUT);
  
  //init
  Wire.begin();
  Serial.begin(115200);
  I2Cinit();
  analogWrite(speedpin,speed);
}

void loop() {
  accelMillis = millis();
  // Check Estop
  Estop();
  // Check door is open
  doorCheck(doorEnabled);
  // read accels
  readAccels();
  // read Raspberry pi comunication
  ReadPi();

  // motor
    float crpm = readRPM(rpmPin);
  if(crpm != -1) {
    avgRpm = 0.3f * avgRpm + 0.7f * crpm;
    Serial.print("SP ");
    Serial.println(avgRpm);
    //Serial.println("SP "+ avgRPM)

    //rpm failsave
    if(avgRpm != setRpm){
    lastCount = counter;
    counter = millis();
    }else{
    counter = 0;
    }
  }

  //Serial.println(counter);
  if(counter - lastCount >= 15000){
    analogWrite(speedpin, 0);
    speed = 0;
    setRpm = 0;
    counter = 0;
  }

  if(avgRpm == 0){
    Serial.println("SG 0");
  }else{
    Serial.println("SG 1");
  }

  long cMillis = millis();
  if(doAutoAdjust && cMillis > lastAutoAdjust + autoAdjustSpace && cMillis > autoAdjustStart) {
    lastAutoAdjust = cMillis;

    if(avgRpm - setRpmBuffer > setRpm) {
      speed -= setRpmChangeVoltage;
      //Serial.print("Adjusted to ");
      //Serial.println(speed);
      analogWrite(speedpin, speed);
    }
    if(avgRpm + setRpmBuffer < setRpm) {
      speed += setRpmChangeVoltage;
      //Serial.print("Adjusted to ");
      //Serial.println(speed);
      analogWrite(speedpin, speed);
    }
  } 
}


void readAccels(){
//Read and send Accel data and time

  //read Sensor 1
  MP.selectChannel(3);
  double G1X = readLSMAccel(GyroAccel1, 'X').toDouble();
  double G1Y = readLSMAccel(GyroAccel1, 'Y').toDouble();
  double G1Z = readLSMAccel(GyroAccel1, 'Z').toDouble();

  //read Sensor 2
  MP.selectChannel(5);
  double G2X = readLSMAccel(GyroAccel2, 'X').toDouble();
  double G2Y = readLSMAccel(GyroAccel2, 'Y').toDouble();
  double G2Z = readLSMAccel(GyroAccel2, 'Z').toDouble();

  //check threshold
  if(G1X >= accelTrheshold ||
     G1Y >= accelTrheshold ||
     G1Z >= accelTrheshold ||
     G2X >= accelTrheshold ||
     G2Y >= accelTrheshold ||
     G2Z >= accelTrheshold ){
    analogWrite(speedpin, 0);
  }

  //return time
  Serial.print("AT ");
  Serial.println(accelMillis - lastAccelMillies);
  //Serial.println("AT " + (accelMillis - lastAccelMillies));
  

  //send Sensor 1 data
  Serial.print("A1 X ");
  Serial.println(G1X);
  //Serial.println("A1 X " + G1X);
  Serial.print("A1 Y ");
  Serial.println(G1Y);
  //Serial.println("A1 Y " + G1Y);
  Serial.print("A1 Z ");
  Serial.println(G1Z);
  //Serial.println("A1 Z " + G1Z);
  //send Sensor 2 data
  Serial.print("A2 X ");
  Serial.println(G2X);
  //Serial.println("A2 X " + G2X);
  Serial.print("A2 Y ");
  Serial.println(G2Y);
  //Serial.println("A2 Y " + G2Y);
  Serial.print("A2 Z ");
  Serial.println(G2Z);
  //Serial.println("A2 Z " + G2Z);
}

void readHumTemp(){
//Read and send Tempature and Humidity data
  MP.selectChannel(1);
  Serial.print("HU ");
  Serial.println(readCHTHumidity(cht1));
  //Serial.println("HU " + readCHTHumidity(cht1));
  Serial.print("TE ");
  Serial.println(readCHTTemperature(cht1));
  //Serial.println("TE " + readCHTTemperature(cht1));
  MP.selectChannel(7);
  Serial.print("HU ");
  Serial.println(readCHTHumidity(cht2));
  //Serial.println("HU " + readCHTHumidity(cht2));
  Serial.print("TE ");
  Serial.println(readCHTTemperature(cht2));
  //Serial.println("TE " + readCHTTemperature(cht2));
}

float readRPM(int rPin) {
//Read and send RPM sensor data
long readDelayRpm;
  if(digitalRead(rPin) == LOW) {
    readr++;

    if(readr == 100) {
      long current = micros();
      long diff = current - lastRot;
      lastRot = current;
      int rpmtmp = (60 * 1000000) / (float)diff;  
      readDelayRpm = millis();
      lastAccelMillies = accelMillis;
      if(rpmtmp < 1000) {
        //Serial.println(rpmtmp);
        rpm = rpmtmp;
        return rpm;
      }
    }
  } else if(digitalRead(rPin) == HIGH) {
    readr--;
    if (readr < 0) readr = 0;
    if(readDelayRpm - lastReadRpm >= 5000){
      lastReadRpm = readDelayRpm;
      return 0;
    }
  }
  return -1;
}

void ReadPi(){
//Read Serial in from Pi
  int endOfData = false;
  int parseFlag = false;
  int i = 0;
  int off = 0;
  char data[11] = "----------";
  char header[6] = "-----";
  char val[6]= "-----";

  //reading serial string
  while(Serial.available() > 0 && endOfData == false){
      char buffer = Serial.read();
      if(buffer != '\n' && i <= 10){
        data[i] = buffer;
      } else {
        endOfData = true;
      }
      i++;
  }

  //parsing data
  for(int j = 0; j <= 10; j++){
    if(parseFlag == false){
      header[j] = data[j];
    } else {
      val[j-off] = data[j];
    }

    if(data[j] == ' '){
      parseFlag = true;
      off = j + 1;
    }
  }
  
  //decode data
  if(not strcmp(header,"STOP ")){
    setRpm = 0;
    speed = 0;
    //write speed
    analogWrite(speedpin, 0);
  } else if(not strcmp(header,"SL --")){
    setRpm = atoi(val);
  } else if(not strcmp(header,"GD --")){
    readHumTemp();
  }
}

void Estop(){
// E-stop Pressed
  if (digitalRead(EstopPin))
  {
    Serial.println("EST 1");
    //stop motor
    analogWrite(speedpin, 0);
    Serial.println("SG 0");
    while (digitalRead(EstopPin)){/*wait while E-stop is pressed*/}
    Serial.println("EST 0");
    analogWrite(speedpin, speed);
  }
}

void doorCheck(int Enabled){
// Door Check
  if (Enabled){
    doorIsOpen = digitalRead(doorPin);
    while (not doorIsOpen &&  not digitalRead(EstopPin))
    {
      Serial.println("DO 1");
      Serial.println("SG 0");
      //stop motor
      analogWrite(speedpin,0);
      readAccels();
      ReadPi();
      doorIsOpen = digitalRead(doorPin);
    }
    Serial.println("DO 0");
    analogWrite(speedpin,speed);
  }
}
