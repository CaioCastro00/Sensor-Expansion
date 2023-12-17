#include <Arduino.h>
#include <vector>
#include <iostream>
#include <cstring>
#include "math.h"
#include "Wire.h"
#include "SerialTransfer.h"
#include "CommunicationManager.h"
#include "PackageManager.h"
#include "TestManager.h"


SerialTransfer myTransfer;


void setup()
{

  
  Serial.begin(115200);
  myTransfer.begin(Serial);
  Buffer buffer(243, 9);

  {
    Timer timer;
    delay(5000);

    for (uint8_t i = 1; i < 10; i++)
    {
      MockPackage1 p;
      p = {25, 200, 1000.0F + i};
      buffer.addItem(p);
    }

    for (uint8_t i = 10; i < 28; i++)
    {
      MockPackage2 p;
      p = {1, 300, 1};
      buffer.addItem(p);
    }
  }

  Serial.println();
  MockPackage1 retrievedPackage1 = buffer.getItem<MockPackage1>(0);
  Serial.print("Retrieved Package:");
  Serial.print(retrievedPackage1.datagram_ID);
  Serial.print(" , Timestamp:");
  Serial.print(retrievedPackage1.timestamp);
  Serial.print(" , Value:");
  Serial.println(retrievedPackage1.value);

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
    Timer timer;
    uint8_t size = myTransfer.sendDatum(buffer.buffer[0], 243);
  }
  // uint8_t size = myTransfer.sendDatum(buffer.buffer.data(), 243);
  // Serial.println();
  // Serial.println(size);
};

void loop() {};