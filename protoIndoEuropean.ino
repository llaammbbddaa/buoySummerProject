// protoIndoEuropean (idk, i think its a funny name, and i like linguistics)
// july fifteenth, johnny sarrouf
// ideally, the prototype for the data collection for the buoy
// github, https://github.com/llaammbbddaa/buoySummerProject/

// components
// geniuno mega
// temp sensor, digi 52
// i2c oxy sensor, scl1 & sda1, 0x73 address
// micro sd card read / write, 53-50
// tds sensor, a15

// all the libraries
#include <Wire.h>
#include <DS18B20.h>
#include <DFRobot_OxygenSensor.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include "CQRobotTDS.h"

// various init stuff
DS18B20 tempSensor(48);
DFRobot_OxygenSensor oxySensor;
File dataWrite;
CQRobotTDS tdsSensor(A15);

void setup() {

  Serial.begin(9600);
  Wire.begin();

  if (!oxySensor.begin(0x73)) {
    Serial.println("oxy sensor dne");
    while (1);
  }

  if (!SD.begin(53)) {
    Serial.println("initialization failed!sfidsajfoj");
    while (1);
  }

}

void fileWrite(String data) {
  String fileName = ("dataBuoy.txt");
  dataWrite = SD.open(fileName, FILE_WRITE);
  if (dataWrite) {
    dataWrite.println(data);
    dataWrite.close();
  }
}

float fToC(float f) {
  return ((f - 32) * (5/9));
}

void loop() {
  // put your main code here, to run repeatedly:

  float temp = tempSensor.getTempF();
  // the ten is there because it takes the averages of the number that you give, the higher the more smooth the value
  float oxy = oxySensor.getOxygenData(10);
  float tds = tdsSensor.update(fToC(temp));
  Serial.println("newData");
  Serial.println(temp);
  Serial.println(oxy);
  Serial.println(tds);

  delay(750);
}
