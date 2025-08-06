// no good comments, in a rush


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
#include "CQRobotTDS.h"
#include <LowPower.h>
#include <AnalogPHMeter.h>
#include "GravityTDS.h"


// various init stuff
DS18B20 tempSensor(4);
DFRobot_OxygenSensor oxySensor;
#define TdsSensorPin A3
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 28;


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

   static float lastTds = 0;
   
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

      lastTds = tdsValue;
   }
   return lastTds;
}


void setup() {

  Serial.begin(9600);

  Wire.begin();

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(TdsSensorPin, INPUT);

//  if (!oxySensor.begin(0x73)) {
//    Serial.println("oxy sensor dne");
//    
//    while (true) {
//      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//      delay(100);                       // wait for a second
//      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//      delay(100);     
//    }
//  }
  

}


void ledBlink() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(250);                       // wait for a second
}




float fToC(float f) {
  return ((f - 32) * (5 / 9));
}

bool waterPresent(int waterVal) {
  return (waterVal > 100);
}
void waterShutoff(bool water) {
  if (water) {
    Serial.println("water detected...");
    // LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
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
  // waterShutoff(waterPresent(analogRead(A1)));

  float temp = tempSensor.getTempC();
  // the ten is there because it takes the averages of the number that you give, the higher the more smooth the value
  float oxy = 20.2; // oxySensor.getOxygenData(10);
  float pH = (analogRead(A0) * .01);
  float tds = getTds();

  //  Serial.println("newData");
  //  Serial.println(temp);
  //  Serial.println(oxy);
  //  Serial.println(tds);
  //  Serial.println((analogRead(A0) * .01));
  //  Serial.println(fileNameUnique);

  // the first piece of data, for the temp sensor, is usually wrong
  // so this is here just to scrap it

  // temp,oxy,tds,pH,algalScore ~ for csv ordering
  String newData = String(temp) + "," + String(oxy) + "," + String(tds) + "," + String(pH) + "," + String(algalBloom(pH, temp, tds));
  Serial.println(newData);
  ledBlink();

  // in order to save on power, instead of delay
  // we can turn the system off for eight second intervals
  // LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

}
