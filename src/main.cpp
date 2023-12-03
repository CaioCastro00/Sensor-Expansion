#include <Arduino.h>
#include "Types.h"
#include "Wire.h"
#include "SerialTransfer.h"

SerialTransfer myTransfer;
uint32_t nextUpdate, timeToComplete;

// typedef Package MAX31855Packet;

MAX31855Packet buffer[28];

void setup()
{
  for (uint8_t i = 0; i < 28; i++)
  {
    // FloatPayload p;
    FloatPayload p  = {i, millis(), 1000.0F + i };
    // {i, millis(), 1000.0F + i }
    buffer[i] = p;
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
    // timeToComplete = micros();

    for (size_t i = 0; i < 38; i++)
    {
      uint8_t size = myTransfer.sendDatum(buffer);
    }
    

    // timeToComplete = micros() - timeToComplete;
    // Serial.print("\nTotal:");
    // Serial.print(timeToComplete);
    // Serial.print(" : size: ");
    // Serial.print(sizeof(SensorPackage));
    // Serial.print(" : sended size: ");
    // Serial.println(size);
    nextUpdate = micros() + 1000000 ;
  }
  // delay(100);
}