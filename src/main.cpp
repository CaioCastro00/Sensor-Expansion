#include <Arduino.h>
#include <vector>
#include <iostream>
#include <cstring>
#include "math.h"
#include "Wire.h"
#include "SerialTransfer.h"
#include "CommunicationManager.h"
#include "PackageManager.h"
#include "PackageTypes.h"
#include "ProfileManager.h"
#include "SensorExpansionManager.h"

uint32_t lastTime = 0;

uint32_t nextUpdate = 0;


SerialTransfer myTransfer;
PackageManager buffer(BUFFER_MAX_SIZE, PACKAGE_SIZE, &myTransfer);

void fillBuffer();

void setup(){

  
  Serial.begin(115200);
  myTransfer.begin(Serial);

  delay(5000);

  {
    ProfileTimer timer("filling");
    
    Serial.println();
    Serial.print(" , isFull:");
    Serial.println(buffer.isFull());
    Serial.print(" , getSize:");
    Serial.println(buffer.getSize());

    fillBuffer();
    lastTime = micros();
    buffer.sendViaSerial();
    lastTime = micros() - lastTime;

    Serial.println("END: ");
    Serial.println(lastTime);

    Serial.println();
    Serial.print(" , isFull:");
    Serial.println(buffer.isFull());
    Serial.print(" , getSize:");
    Serial.println(buffer.getSize());

  }

  // Serial.println();
  // MockPackage1 retrievedPackage1 = buffer.getItem<MockPackage1>(0);
  // Serial.print("Retrieved Package:");
  // Serial.print(retrievedPackage1.datagram_ID);
  // Serial.print(" , Timestamp:");
  // Serial.print(retrievedPackage1.timestamp);
  // Serial.print(" , Value:");
  // Serial.println(retrievedPackage1.value);
  // Serial.print(" , isFull:");
  // Serial.println(buffer.isFull());
  // Serial.print(" , getSize:");
  // Serial.println(buffer.getSize());

  // std::cout << "Retrieved Package: " << retrievedPackage1.datagram_ID << ", Value: " << retrievedPackage1.value << std::endl;
  // printf("%f\n", retrievedPackage1.value);

  // MockPackage2 retrievedPackage2 = buffer.getItem<MockPackage2>(234);
  // std::cout << "Retrieved Package: " << retrievedPackage2.datagram_ID << ", Value: " << retrievedPackage2.value << std::endl;
  // printf("%i\n", retrievedPackage2.value);

  // Serial.print("Retrieved Package:");
  // Serial.print(retrievedPackage2.datagram_ID);
  // Serial.print(" , Timestamp:");
  // Serial.print(retrievedPackage2.timestamp);
  // Serial.print(" , Value:");
  // Serial.println(retrievedPackage2.value);

  // Serial.print("isFull: ");
  // Serial.println(buffer.isFull());

  // auto exitBuffer;
  // std::memcpy(&exitBuffer, &buffer.buffer[0], 243);
  {
    ProfileTimer timer("transfer");
    uint8_t size = myTransfer.sendDatum(buffer.buffer[0], 243);
  }

  Serial.print(" , getSize:");
  Serial.println(buffer.getSize());
  // uint8_t size = myTransfer.sendDatum(buffer.buffer.data(), 243);
  // Serial.println();
  // Serial.println(size);
};

void fillBuffer(){

  for (uint16_t i = 0; i < 500; i++)
  {
    Uint32Payload p;
    p.datagramID = Datagram::ADS1256_PT_01;
    p.timestamp = millis();
    p.value = i;
    buffer.addItem(p);
  }

  for (uint16_t i = 0; i < 11; i++)
  {
    FloatPayload p;
    p.datagramID =  Datagram::MAX31855_TT_C1;
    p.timestamp = millis();
    p.value = 500.0F + i;
    buffer.addItem(p);
  }

  for (uint16_t i = 0; i < 500; i++)
  {
    Uint8Payload p;
    p.datagramID = Datagram::VALVE_STATE_SPV_M1;
    p.timestamp = millis();
    p.value = i % 2 == 0 ? 1 : 0;
    buffer.addItem(p);
  }
}

void loop(){

  unsigned long start = millis();

  // if (millis() - avionicsManager.getRate4Interval() > 1000 / 4)
  // {
  //   
  //   avionicsManager.setRate4Interval(millis());
  // }

  // if (millis() - avionicsManager.getRate10Interval() > 1000 / 10)
  // {
  //  
  //   avionicsManager.setRate10Interval(millis());
  // }

  // if (millis() - avionicsManager.getRate80Interval() > 1000 / 80)
  // {
  //   
  //   avionicsManager.setRate80Interval(millis());
  // }

  // if (millis() - avionicsManager.getRate100Interval() > 1000 / 100)
  // {
  //   
  //   avionicsManager.setRate100Interval(millis());
  // }

  // if (millis() - avionicsManager.getRate1000Interval() > 1000 / 1000)
  // {
  //   
  //   avionicsManager.setRate1000Interval(millis());
  // }


  // avionicsManager.writeSD();
  // avionicsManager.flushDataFile();
  
  unsigned long end = millis();
  Serial.println(end-start);

};