#include <Arduino.h>
#include <vector>
#include <iostream>
#include "math.h"
#include "Wire.h"
#include "SerialTransfer.h"
#include "CommunicationManager.h"
#include "PackageManager.h"
#include "PackageTypes.h"
#include "SensorExpansionManager.h"

uint32_t lastTime = 0;

uint32_t nextUpdate = 0;

SensorExpansionManager sensorExpansionManager;
SerialTransfer myTransfer;
PackageManager buffer(BUFFER_MAX_SIZE, PACKAGE_SIZE, &myTransfer);

// void fillBuffer();

void setup(){

  Serial.begin(115200);
  myTransfer.begin(Serial);
  Serial.println("Initializing...");
  sensorExpansionManager.init();
  Serial.println("Initialized!");
  // fillBuffer();
};


void loop(){

  unsigned long start = millis();

  // if (millis() - sensorExpansionManager.getRate4Interval() > 1000 / 4)
  // {
    
  //   sensorExpansionManager.setRate4Interval(millis());
  // }

  if (millis() - sensorExpansionManager.getRate10Interval() > 1000 / 10)
  {
    sensorExpansionManager.getMAX31855data();
    sensorExpansionManager.updateMAX31855data(27);    
    sensorExpansionManager.setRate10Interval(millis());
  }

  if (millis() - sensorExpansionManager.getRate80Interval() > 1000 / 80)
  {
    sensorExpansionManager.getHX711data();
    sensorExpansionManager.updateHX711data(25);
    sensorExpansionManager.setRate80Interval(millis());
  }

  // if (millis() - sensorExpansionManager.getRate100Interval() > 1000 / 100)
  // {
    
  //   sensorExpansionManager.setRate100Interval(millis());
  // }

  if (millis() - sensorExpansionManager.getRate1000Interval() > 1000 / 1000)
  {
    sensorExpansionManager.getADS1256data();
    sensorExpansionManager.setRate1000Interval(millis());
  }

  buffer.sendViaSerial();
  uint8_t size = myTransfer.sendDatum(buffer.buffer[0], 243);
  
  unsigned long end = millis();
  Serial.println(end-start);

};