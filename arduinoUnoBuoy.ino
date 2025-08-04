// arduinoUnoBuoy.ino
// this one is for the arduino Uno
// things APPEAR to be working as intended
// but because the uno is not as powerful as the mega, performance issues may occur

// OLD NAME ~~protoIndoEuropean (idk, i think its a funny name, and i like linguistics)~~// july fifteenth, johnny sarrouf
// ideally, the prototype for the data collection for the buoy
// github, https://github.com/llaammbbddaa/buoySummerProject/

// components
// arduino uno
// temp sensor, d4
// i2c oxy sensor, scl0 & sda0, (a4 and a5), 0x73 address
// micro sd card read / write, 13-10
// tds sensor, a2
// pH sensor, a0
// water sensor, a1

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
DS18B20 tempSensor(4);
DFRobot_OxygenSensor oxySensor;
File dataWrite;
CQRobotTDS tdsSensor(A2);
String fileNameUnique;
bool firstData;

void setup() {

  Serial.begin(9600);
  Wire.begin();

  if (!oxySensor.begin(0x73)) {
    Serial.println("oxy sensor dne");
    while (1);
  }

  if (!SD.begin(10)) {
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

// from zero to one, technically not a percentage, but i prefer it this way
float percentDiff(float a, float b) {
  return ((abs(a - b)) / ((a + b) / 2));
}

// supposed to return a probability of an algal bloom occurring, given certain data
float algalBloom(float pH, float temp, float tds) {
  pH = percentDiff(pH, 8);
  temp = percentDiff(temp, 25);
  tds = percentDiff(tds, 35000);

  // INCREDIBLY INCREDIBLY basic idea here, a one would be optimal setting for an algal bloom
  // any reasonable deviation from the optimal setting for algae reduces the one by a factor determined by the percent difference of said algal efficiency
  // tested with data sets from the gulf of mexico to determine a percent threshold of about sixty eight percent
  float prob = 100 * (1 - (pH + temp + tds));
  if (prob >= 68) {
    return 1;
  }
  return 0;
}

void loop() {
  // put your main code here, to run repeatedly:

  // checks to see if there is water within the container, and if so , it turns the arduino off indefinitely as a last resort
  waterShutoff(waterPresent(analogRead(A1)));

  float temp = tempSensor.getTempC();
  // the ten is there because it takes the averages of the number that you give, the higher the more smooth the value
  float oxy = oxySensor.getOxygenData(10);
  float tds = tdsSensor.update(temp);
  float pH = (analogRead(A0) * .01);
  
//  Serial.println("newData");
//  Serial.println(temp);
//  Serial.println(oxy);
//  Serial.println(tds);
//  Serial.println((analogRead(A0) * .01));
//  Serial.println(fileNameUnique);

  // the first piece of data, for the temp sensor, is usually wrong
  // so this is here just to scrap it
  if (firstData) {
    firstData = false;
  }
  else {
    // temp,oxy,tds,pH,algalScore ~ for csv ordering
    String newData = String(temp) + "," + String(oxy) + "," + String(tds) + "," + String(pH) + "," + String(algalBloom(pH, temp, tds));
    fileWrite(newData);
  }

  // delay(750);
  // in order to save on power, instead of delay
  // we can turn the system off for eight second intervals
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  
}
