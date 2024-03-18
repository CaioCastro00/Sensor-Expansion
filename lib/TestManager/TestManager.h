#pragma once

#include <Arduino.h>
#include "CommunicationManager.h"
#include "PackageManager.h"
#include "ReportManager.h"
#include "SensorExpansionManager.h"

class TestManager{

public:

    void TestManager::init(){

    initializeI2C();
//   initializeADC();
    // initializeINA();
//   initializeBMP388(BMP3_NO_OVERSAMPLING, BMP3_NO_OVERSAMPLING, BMP3_IIR_FILTER_DISABLE, BMP3_ODR_200_HZ);
}

    void initializeI2C(){
        Wire.begin();

        while (!Serial); 
        Serial.println("\nI2C Scanner");
    }

    void scanI2C(){
        byte error, address;
        int nDevices;

        Serial.println("Scanning...");

        nDevices = 0;
        for (address = 1; address < 127; address++)
        {
            // The i2c_scanner uses the return value of
            // the Write.endTransmisstion to see if
            // a device did acknowledge to the address.
            Wire.beginTransmission(address);
            error = Wire.endTransmission();

            if (error == 0)
            {
                Serial.print("I2C device found at address 0x");
                if (address < 16)
                    Serial.print("0");
                Serial.print(address, HEX);
                Serial.println("  !");

                nDevices++;
            }
            else if (error == 4)
            {
                Serial.print("Unknown error at address 0x");
                if (address < 16)
                    Serial.print("0");
                Serial.println(address, HEX);
            }
        }
        if (nDevices == 0)
            Serial.println("No I2C devices found\n");
        else
            Serial.println("done\n");

        delay(5000); // wait 5 seconds for next scan
    }

private:


};