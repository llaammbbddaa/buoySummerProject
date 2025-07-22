// arduinoMegaBuoy.ino
// this one is for the arduino MEGA, but was eventually changed to a regular arduino uno as to save on size and battery
// the code is fully functional as far as i am concerned
// OLD NAME ~~protoIndoEuropean (idk, i think its a funny name, and i like linguistics)~~
// july fifteenth, johnny sarrouf
// ideally, the prototype for the data collection for the buoy
// github, https://github.com/llaammbbddaa/buoySummerProject/

// components
// geniuno mega
// temp sensor, digi 52
// i2c oxy sensor, scl1 & sda1, 0x73 address
// micro sd card read / write, 53-50
// tds sensor, a15
// pH sensor, a1
// water sensor, a7

// all the libraries
#include <Wire.h>
#include <DS18B20.h>
#include <DFRobot_OxygenSensor.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include "CQRobotTDS.h"
#include <LowPower.h>
#include <AnalogPHMeter.h>

// various init stuff
DS18B20 tempSensor(48);
DFRobot_OxygenSensor oxySensor;
File dataWrite;
CQRobotTDS tdsSensor(A15);
String fileNameUnique;
bool firstData;

void setup() {

  Serial.begin(9600);
  Wire.begin();

  if (!oxySensor.begin(0x73)) {
    Serial.println("oxy sensor dne");
    while (1);
  }

  if (!SD.begin(53)) {
    Serial.println("sd dne");
    while (1);
  }

  fileNameUnique = fileNameCreate();
  firstData = true;

}

// NOT USED
// tries to read from file, but if the file dne, then returns false
// for checking unique file names in sd dir
bool fileExist(String fileName) {
  File testFile = SD.open(fileName, FILE_READ);
  if (testFile) {
    testFile.close();
    return false;
  }
  return true;
}

// NOT USED
// generates appropriate name for file
String fileNameCreate() {
  String fileName = ("dataBuoy.txt");
  int counter = 1;
  while (1) {
    if (fileExist(fileName)) {
      return fileName;
    }
    fileName = "dataBuoy" + String(counter) + ".txt";
  }
}

void fileWrite(String data) {

  // a unique file name didnt really end up making sense
  // especially since it wouldnt even be used if the thing is always running
  fileNameUnique = "dataBuoy.txt";
  
  dataWrite = SD.open(fileNameUnique, FILE_WRITE);
  if (dataWrite) {
    dataWrite.println(data);
    dataWrite.close();
  }
}

float fToC(float f) {
  return ((f - 32) * (5/9));
}

bool waterPresent(int waterVal) {
  return (waterVal > 100);
}
void waterShutoff(bool water) {
  if (water) {
    // Serial.println("water detected...");
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // checks to see if there is water within the container, and if so , it turns the arduino off indefinitely as a last resort
  waterShutoff(waterPresent(analogRead(A7)));

  float temp = tempSensor.getTempF();
  // the ten is there because it takes the averages of the number that you give, the higher the more smooth the value
  float oxy = oxySensor.getOxygenData(10);
  float tds = tdsSensor.update(fToC(temp));
  
//  Serial.println("newData");
//  Serial.println(temp);
//  Serial.println(oxy);
//  Serial.println(tds);
//  Serial.println((analogRead(A1) * .01));
//  Serial.println(fileNameUnique);

  // the first piece of data, for the temp sensor, is usually wrong
  // so this is here just to scrap it
  if (firstData) {
    firstData = false;
  }
  else {
    String newData = String(temp) + "," + String(oxy) + "," + String(tds) + "," + String((analogRead(A1) * .01));
    fileWrite(newData);
  }

  // delay(750);
  // in order to save on power, instead of delay
  // we can turn the system off for eight second intervals
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  
}
