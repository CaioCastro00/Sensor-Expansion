#include <Arduino.h>
#include "Types.h"
#include "Wire.h"
#include "SerialTransfer.h"
#include "PackageManager.h"
#include "test.h"

SerialTransfer myTransfer;
uint32_t nextUpdate, timeToComplete;

// typedef Package MAX31855Packet;

// MAX31855Packet buffer[28];
PacketBuffer buffer(5);

void setup()
{
  // for (uint8_t i = 0; i < 28; i++)
  // {
  //   // FloatPayload p;
  //   // FloatPayload p  = {i, millis(), 1000.0F + i };
  //   // {i, millis(), 1000.0F + i }
  //   // buffer[i] = p;
  // }


  for (int i = 0; i < 8; ++i) {
        // Use the constructor to create FloatPayload
        FloatPayload* packet = new FloatPayload{static_cast<uint8_t>(i), static_cast<uint32_t>(i * 1000), 3.14f + i};
        buffer.addPacket(packet, 9);
  }

  Serial.begin(115200);
  myTransfer.begin(Serial);
  nextUpdate = micros();
  delay(5000);

  // y = 4.5;
}

void loop()
{

  // micros()
  if (micros() >= nextUpdate)
  {
    // Serial.println(sizeof(SensorPackage));
    timeToComplete = micros();

    // for (size_t i = 0; i < 5; i++)
    // {
    uint8_t size = myTransfer.sendDatum(buffer);
    // }
    

    timeToComplete = micros() - timeToComplete;
    Serial.print("\nTotal:");
    Serial.print(timeToComplete);
    // Serial.print(" : size: ");
    // Serial.print(sizeof(SensorPackage));
    Serial.print(" : sended size: ");
    Serial.println(size);
    nextUpdate = micros() + 1000000 ;
  }
  // delay(100);
}