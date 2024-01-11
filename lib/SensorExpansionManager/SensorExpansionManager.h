#include <Arduino.h>
#include <Adafruit_INA219.h>
#include <Adafruit_MAX31855.h>
#include <ADS1256.h>
#include <HX711.h>
#include "CommunicationManager.h"
#include "PackageManager.h"
#include "ProfileManager.h"
#include "ReportManager.h"
#include "consts.h"


class SensorExpansionManager{

public:

    void init(){
        initializeADS1256();
        initializeHX711();
        initializeMAX31855();
        initializeINA();
    }

    bool initializeADS1256(){
        ads1256.setupADC(ADS1256_DRDY, ADS1256_RST, ADS1256_PDWN, ADS1256_CS, 2.500); //DRDY, RESET, SYNC(PDWN), CS, VREF(float).
        ads1256.InitializeADC();
        return true;
    }

    bool initializeHX711(){
        hx711.begin(HX711_DOUT_PIN, HX711_SCK_PIN);
        hx711.set_scale(CALIBRATION_FACTOR);
        hx711.tare();
        return true;
    }

    bool initializeMAX31855(){
        max31855.setup(MAX31855_SCK_PIN, MAX31855_CS_PIN, MAX31855_MISO_PIN); // int8_t _sclk, int8_t _cs, int8_t _miso
        return max31855.begin();
    }

    bool initializeINA(){
        if (!ina219.begin()){
            return false;
        }
        return true;
    }

    void getADS1256data(){

        for (int i = 0; i < PORTS; i++)
        {
            _ads1256data = ads1256.convertToVoltage(ads1256.cycleSingle());
            updateADS1256data(i+5);
        }
       
        ads1256.stopConversion();
    }

    void updateADS1256data(int id){
        ads1256package.datagramID =  (Datagram) id;
        ads1256package.timestamp = millis();
        ads1256package.value = _ads1256data;
        buffer.addItem(ads1256package);
    }

    void getHX711data(){
        _hx711data = hx711.get_units();
    }

    void updateHX711data(int id){
        hx711package.datagramID =  (Datagram) id;
        hx711package.timestamp = millis();
        hx711package.value = _hx711data;
        buffer.addItem(hx711package);
    }

    void getMAX31855data(){

    }

    void updateMAX31855data(){

    }

    void getINAdata(){

    }

    void updateINAdata(){

    }


private:

    Adafruit_INA219 ina219;
    Adafruit_MAX31855 max31855;
    ADS1256 ads1256;
    HX711 hx711;
    // thermistor
    // valvestate

    ADS1256Package ads1256package;
    HX711Package hx711package;
    INAPackage inapackage;
    MAX31855Package max31855package;
    ThermistorPackage thermistorpackage;
    ValveStatePackage valvestatepackage;

    uint8_t _valvestate;
    float_t _hx711data;
    float_t _inadata;
    float_t _max31855data;
    float_t _thermistordata;
    uint32_t _ads1256data;

    bool _reading_ADS;
    bool _reading_HX;
    bool _reading_INA;
    bool _reading_MAX;

    unsigned long _updateInterval_ADS;
    unsigned long _updateInterval_HX;
    unsigned long _updateInterval_INA;
    unsigned long _updateInterval_MAX;


};