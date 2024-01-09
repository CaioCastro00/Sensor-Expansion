#include <Arduino.h>
#include <Adafruit_INA219.h>
#include <ADS1256.h>
#include <HX711.h>
#include "CommunicationManager.h"
#include "PackageManager.h"
#include "ProfileManager.h"
#include "ReportManager.h"
#include "consts.h"


// ADS1256 ads1256(8, 10, 5, 9, 2.500); //DRDY, RESET, SYNC(PDWN), CS, VREF(float)

class SensorExpansionManager{

public:

    void init(){
        ads1256.InitializeADC(); //DRDY, RESET, SYNC(PDWN), CS, VREF(float).
        hx711.begin(HX711_DOUT_PIN, HX711_SCK_PIN);
    }

    bool verifyInterval(unsigned long _time, unsigned long _interval){
        return micros() - _time >= _interval;
    }

    bool initializeADS1256(){
        return true;
    }

    void updateADS1256data(){

        for (int j = 0; j < 4; j++){
            ads1256.convertToVoltage(ads1256.cycleDifferential());
        }
        
        ads1256.stopConversion();
    }

    bool initializeHX711(){
        return true;
    }

    // getHX711data(){}

    void updateHX711data(){

    }

    bool initializeINA(){
        if (!ina219.begin()){
            Serial.println("INA error");
            return false;
        }
        return true;
    }

    // getINAdata(){}
    
    void updateINAdata(){

    }


    void ads1256_function(){
        if (Serial.available() > 0)
        {
            char commandCharacter = Serial.read(); //we use characters (letters) for controlling the switch-case

            switch (commandCharacter) //based on the command character, we decide what to do
            {
                //--------------------------------------------------------------------------------------------------------
                case 'D': //Cycle differential inputs (A0+A1, A2+A3, A4+A5, A6+A7)
                    while (Serial.read() != 's') //The conversion is stopped by a character received from the serial port
                    {
                    for (int j = 0; j < 4; j++)
                    {
                        Serial.print(ads1256.convertToVoltage(ads1256.cycleDifferential()), 4);//print the converted differential results with 4 digits
                        Serial.print("\t"); //tab separator to separate the 4 conversions shown in the same line
                    }
                    Serial.println(" ");//Printing a linebreak - this will put the next 4 conversions in a new line
                    }
                    ads1256.stopConversion();
                    break;
                //--------------------------------------------------------------------------------------------------------
                case 'B': //Speed test
                    {
                    ads1256.setDRATE(DRATE_30000SPS); //Set speed to 300000 sps
                    ads1256.setMUX(DIFF_6_7); //Set input to A6(+) & A7(-)

                    //Variables to store and measure elapsed time and define the number of conversions
                    long numberOfSamples = 150000; //Number of conversions
                    long finishTime = 0;
                    long startTime = micros();

                    for (long i = 0; i < numberOfSamples; i++)
                    {
                        ads1256.readSingleContinuous();            
                        //Note: here we just perform the readings and we don't print the results
                    }

                    finishTime = micros() - startTime; //Calculate the elapsed time

                    ads1256.stopConversion();

                    //Printing the results
                    Serial.print("Total conversion time for 150k samples: ");
                    Serial.print(finishTime);
                    Serial.println(" us");

                    Serial.print("Sampling rate: ");
                    Serial.print(numberOfSamples * (1000000.0 / finishTime), 3);
                    Serial.println(" SPS");
                    }
                    break;
                //--------------------------------------------------------------------------------------------------------
            }
        }
    }





private:

    Adafruit_INA219 ina219;
    ADS1256 ads1256;
    HX711 hx711;

    bool _reading_ADS;
    bool _reading_HX;

    unsigned long _updateInterval_ADS;
    unsigned long _updateInterval_HX;




};