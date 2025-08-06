// eepromFix.ino

// basically what happened, is i fried our sd card reader / writer, and i do not have the time or money for another one
// so this is our temporary solution
// an arduino mega because it has four times as much eeprom storage
// everything is about the same other than the parts where we read / write from eeprom
// would not recommend this at all tho, as it seems it really takes a toll on the chip with extended use
// but this will not be used extensively, just until we can get an sd reader / writer
// a lot of this code about the eeprom encoding was not written by me
// my understanding of lower level esque programming isnt great

// all libraries
#include <EEPROM.h>
#include <stdint.h>
#include <Wire.h>
#include <DS18B20.h>
#include <DFRobot_OxygenSensor.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include "CQRobotTDS.h"
#include <LowPower.h>
#include <AnalogPHMeter.h>
#include "GravityTDS.h"

// various init stuff
DS18B20 tempSensor(4);
DFRobot_OxygenSensor oxySensor;
File dataWrite;
//CQRobotTDS tdsSensor(A2);
String fileNameUnique;
bool firstData;
int index = 0;
int totalSize = 4000; // flattened value so it doesnt accidentally overflow and cause issues later on
// GravityTDS gravityTds; doesnt work
#define TdsSensorPin A3
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 28;

//–– Helper: print a uint64_t in decimal via Serial ––
void printUint64(uint64_t x) {
  if (x == 0) {
    Serial.println('0');
    return;
  }
  char buf[21];
  buf[20] = '\0';
  int pos = 20;
  while (x > 0 && pos > 0) {
    buf[--pos] = '0' + (x % 10);
    x /= 10;
  }
  Serial.println(&buf[pos]);
}

//–– Rebuild a uint64_t from 8 big-endian bytes ––
uint64_t bytes8ToUint64(const byte* b) {
  uint64_t v = 0;
  for (int i = 0; i < 8; i++) {
    v = (v << 8) | b[i];
  }
  return v;
}

//–– Turn a uint64_t into a decimal String (if ever needed) ––
String uint64ToString(uint64_t x) {
  if (x == 0) return "0";
  char buf[21];
  buf[20] = '\0';
  int pos = 20;
  while (x > 0 && pos > 0) {
    buf[--pos] = '0' + (x % 10);
    x /= 10;
  }
  return String(&buf[pos]);
}

// read previous data, will be implemented later to extract data from mega
void getSection(int offset) {
  byte dataBytes[8];
  for (int i = 0; i < 8; i++) {
    dataBytes[i] = EEPROM.read(i + offset);
  }

  // outputs the reconstructed encoded int, will be decoded elsewhere eventually...
  Serial.println(String(offset) + ": " + uint64ToString(bytes8ToUint64(dataBytes)));
}

/**
   Writes a 64-bit unsigned integer to EEPROM.

   @param startAddr  The EEPROM address at which to begin writing (0–EEPROM_SIZE-8).
   @param value      The uint64_t value to store.
*/
void writeUint64ToEEPROM(int startAddr, uint64_t value) {
  // Break the 64-bit value into 8 bytes, MSB first
  for (int i = 0; i < 8; i++) {
    byte b = (value >> (8 * (7 - i))) & 0xFF;
    EEPROM.write(startAddr + i, b);
  }
}

uint64_t encodeDecimalString(const String &s) {
  uint64_t v = 0;
  for (char c : s) {
    if (c < '0' || c > '9') continue;
    v = v * 10 + (c - '0');
  }
  return v;
}

// Function to encode sensor data and return 8 bytes
uint64_t dataEncoding(float temp = 0.0, float oxy = 0.0, float tds = 0.0, float pH = 0.0, int algal = 0) {
  int givens[] = { int(temp), int(oxy), int(tds), int(pH), algal };
  for (int i = 0; i < 5; i++) {
    Serial.println(givens[i]);
  }

  String tS, oS, tdS, pS, aS;

  tS = String(givens[0]).substring(0, 2);
  if (givens[0] <= 10) {
    tS = "00";
  }
  
  oS = String(givens[1]).substring(0, 2);
  if (givens[1] <= 10) {
    oS = "00";
  }
  
  tdS = String(givens[2]).substring(0, 3);
  if (givens[2] <= 0) {
    tdS = "000";
  }
  
  if (givens[3] >= 10) {
    pS = String(givens[3]).substring(0, 2);
  }
  else {
    pS = "0" + String(givens[3]);
  }
  if (givens[3] <= 0) {
    pS = "00";
  }
  
  aS = String(givens[4]);

  String allPieces = tS + oS + tdS + pS + aS;
  return encodeDecimalString(allPieces);  // still better to parse manually
}

// also example stuff
int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
      for (i = 0; i < iFilterLen - j - 1; i++) 
          {
        if (bTab[i] > bTab[i + 1]) 
            {
        bTemp = bTab[i];
            bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
         }
      }
      }
      if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
      else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}

// ignore the spacing consistencies, this is example code
float getTds() {
   static unsigned long analogSampleTimepoint = millis();
   if(millis()-analogSampleTimepoint > 40U)     //every 40 milliseconds,read the analog value from the ADC
   {
     analogSampleTimepoint = millis();
     analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
     analogBufferIndex++;
     if(analogBufferIndex == SCOUNT) 
         analogBufferIndex = 0;
   }   
   static unsigned long printTimepoint = millis();
   if(millis()-printTimepoint > 800U)
   {
      printTimepoint = millis();
      for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
        analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
      //Serial.print("voltage:");
      //Serial.print(averageVoltage,2);
      //Serial.print("V   ");
//      Serial.print("TDS Value:");
//      Serial.print(tdsValue,0);
//      Serial.println("ppm");

      return tdsValue;
   }
   return 0;
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  Wire.begin();

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(TdsSensorPin, INPUT);

//  gravityTds.setPin(A3);
//  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
//  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
//  gravityTds.begin();  //initialization

  if (!oxySensor.begin(0x73)) {
    Serial.println("oxy sensor dne");
    while (1);
  }

//  if (!SD.begin(10)) {
//    Serial.println("sd dne");
//    while (1);
//  }

}

void ledBlink() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(250);                       // wait for a second
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
int algalBloom(float pH, float temp, float tds) {
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
  // float tds = tdsSensor.update(temp);
  float pH = (analogRead(A0) * .01);
//  gravityTds.setTemperature(temp);  // set the temperature and execute temperature compensation
//  gravityTds.update();  //sample and calculate
//  float tds = gravityTds.getTdsValue();  // then get the value
  float tds = getTds();
  
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

    // not using this for now, i completely ruined our sd card reader and have to use eeprom as a last resort
//    String newData = String(temp) + "," + String(oxy) + "," + String(tds) + "," + String(pH) + "," + String(algalBloom(pH, temp, tds));
//    fileWrite(newData);

    // using this system instead, because it allows us to write data to the arduino without an sd card
    uint64_t dataEncoded = (dataEncoding(temp, oxy, tds, pH, algalBloom(pH, temp, tds)));
    writeUint64ToEEPROM(index, dataEncoded);
    

    // eight bytes written, so we increment by eight
    index += 8;
    if (index >= 4000) {
      while(1); // stopping the program once the eeprom memory is full (near full)
    }
  }

  //delay(750);
  ledBlink();

  // normally, this would make sense to utilize, but just because we are running out of time
  // and i am not in the mood to tinker with more stuff today, we will just use delay
  // this is two guys in a garage, not nasa
  
  // in order to save on power, instead of delay
  // we can turn the system off for eight second intervals
  // LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  
}
