#include "config.h"
#include "CHT.h"
#include "I2C_Hub.h"
#include "LSM.h"
#include "string.h"
#include "Wire.h"
#include "TCA9548.h"
//////////////////////////////////////////JUNK VARIABLES REMOVE UNUSEDD ONES///////////////////////////////
const int ledsPin = 13;
const int pumpPin = 12;
int ledhigh = LOW;
int pumphigh = LOW;
char command;
char dataIn[20];
int datacounter = 0;
bool reset = false;

char dtaUart[15];
char dtaLen = 0;
uint8_t Data[100] = {0};
uint8_t buff[100] = {0};

uint8_t buf[4] = {0};
uint16_t data, data1;
#define ADDRESS_SENSOR 0x40
////////////////////////////////////////////////////////////////////////




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
long autoAdjustSpace = 1000;
long autoAdjustStart = 3000;
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

TCA9548 MP(0x70);

// CHT8305 cht1(0x40);
// CHT8305 cht2(0x40); 

LSM6DS3 GyroAccel1(I2C_MODE,0x6A);
LSM6DS3 GyroAccel2(I2C_MODE,0x6A);

void setup() {
  //pin setup
  pinMode(speedpin, OUTPUT);
	pinMode(rpmPin, INPUT);
  pinMode(doorPin, INPUT);
  pinMode(EstopPin, INPUT);
  
  //init
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
//  readAccels();
  // read Raspberry pi comunication
  ReadPi();

  // motor
    float crpm = readRPM(rpmPin);
  if(crpm != -1) {
    avgRpm = 0.3f * avgRpm + 0.7f * crpm;
    Serial.print("SP ");
    Serial.println(avgRpm);

    //rpm failsave
    if(avgRpm != setRpm){
    lastCount = counter;
    counter = millis();
    }else{
    counter = 0;
    }
  }

  if(counter - lastCount >= 15000){
    analogWrite(speedpin, 0);
    speed = 0;
    setRpm = 0;
    counter = 0;
  }

//  if(avgRpm == 0){
//    Serial.println("SG 0");
//  }else{
//    Serial.println("SG 1");
//  }
  if(setRpm == 0 && avgRpm < 20) {
    speed = 0;
    doAutoAdjust = 0;
  } else {
    doAutoAdjust = 1;
  }

  long cMillis = millis();
  if(doAutoAdjust && cMillis > lastAutoAdjust + autoAdjustSpace && cMillis > autoAdjustStart) {
    lastAutoAdjust = cMillis;

    if(avgRpm - setRpmBuffer > setRpm) {
      speed -= setRpmChangeVoltage;
      if(speed < 0) {
        speed = 0;
      }
      analogWrite(speedpin, speed);
    }
    if(avgRpm + setRpmBuffer < setRpm) {
      speed += setRpmChangeVoltage;
      if(speed > 254) {
        speed = 254;
      }
      analogWrite(speedpin, speed);
    }
  } 
}


void readAccels(){
//Read and send Accel data and time

  //read Sensor 1
  //MP.selectChannel(2);
  TCA9548A(2);
  double G1X = readLSMAccel(GyroAccel1, 'X').toDouble();
  double G1Y = readLSMAccel(GyroAccel1, 'Y').toDouble();
  double G1Z = readLSMAccel(GyroAccel1, 'Z').toDouble();

  //read Sensor 2
  // MP.selectChannel(5);
  TCA9548A(5);
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

  //send Sensor 1 data
  Serial.print("A1 X ");
  Serial.println(G1X);
  Serial.print("A1 Y ");
  Serial.println(G1Y);
  Serial.print("A1 Z ");
  Serial.println(G1Z);
  //send Sensor 2 data
  Serial.print("A2 X ");
  Serial.println(G2X);
  Serial.print("A2 Y ");
  Serial.println(G2Y);
  Serial.print("A2 Z ");
  Serial.println(G2Z);
}

void readHumTemp(){
//Read and send Tempature and Humidity data
  TCA9548A(7);
  Serial.print("HU ");
  Serial.println(readSensorDataHum(ADDRESS_SENSOR));
  // Serial.println(cht1.readHumidity());
  Serial.print("TE ");
  Serial.println(readSensorDataTemp(ADDRESS_SENSOR));
  // Serial.println(cht1.readTemperature());
  TCA9548A(1);
  Serial.print("HU ");
  Serial.println(readSensorDataHum(ADDRESS_SENSOR));
  // Serial.println(cht2.readHumidity());
  Serial.print("TE ");
  Serial.println(readSensorDataTemp(ADDRESS_SENSOR));
  // Serial.println(cht2.readTemperature());
}

float readSensorDataHum(uint8_t sensorAddress) {
    readReg(sensorAddress, 0x00, buf, 4);
    data1 = buf[2] << 8 | buf[3];
    return ((float)data1 / 65535.0) * 100;
}

float readSensorDataTemp(uint8_t sensorAddress) {
    readReg(sensorAddress, 0x00, buf, 4);
    data = buf[0] << 8 | buf[1];
    return ((float)data * 165 / 65535.0) - 40.0;
}

uint8_t readReg(uint8_t sensorAddress, uint8_t reg, const void* pBuf, size_t size) {
    if (pBuf == NULL) {
        return 0;
    }
    
    uint8_t* _pBuf = (uint8_t*)pBuf;
    Wire.beginTransmission(sensorAddress);
    Wire.write(&reg, 1);  // Send the register address
    
    if (Wire.endTransmission() != 0) {
        return 0;  // If transmission fails, return 0
    }
    
    delay(20);
    Wire.requestFrom(sensorAddress, (uint8_t)size);
    
    for (uint16_t i = 0; i < size; i++) {
        _pBuf[i] = Wire.read();  // Read the data into the buffer
    }
    
    return size;
}






float readRPM(int rPin) {
//Read and send RPM sensor data
long readDelayRpm;
  if(digitalRead(rPin) == LOW) {
    readr++;

    if(readr == 10) {
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
//      readAccels();/
      ReadPi();
      doorIsOpen = digitalRead(doorPin);
    }
    Serial.println("DO 0");
    analogWrite(speedpin,speed);
  }
}

void I2Cinit() {
  Wire.begin();
    
  //  initialize multiplexer
  if (MP.begin() == false)
  {
    Serial.println("Multiplexer error");
  }
  // initialize the i2c sensors
  TCA9548A(2);
  if (MP.isConnected(0x6A) == false){
    Serial.println("gyro1 not connected");
  } 
  if (GyroAccel1.begin() == false)
  {
    Serial.println("gyro1 error");
  }
  TCA9548A(5);
  if (MP.isConnected(0x6A) == false){
    Serial.println("gyro2 not connected");
  } 
  if (GyroAccel2.begin() == false)
  {
    Serial.println("gyro2 error");
  }
  TCA9548A(7);
  if (MP.isConnected(0x40) == false){
    Serial.println("temp1 not connected");
  } 
  // if (cht1.begin() == false )
  // {
  //   Serial.println("temp1 error");
  // }
  TCA9548A(1);
  if (MP.isConnected(0x40) == false){
    Serial.println("temp2 not connected");
  } 
  // if (cht2.begin() == false )
  // {
  //   Serial.println("temp2 error");
  // }
}

// Select I2C BUS
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}