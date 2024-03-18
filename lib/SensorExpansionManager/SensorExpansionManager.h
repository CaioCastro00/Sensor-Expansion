#pragma once
#ifndef SENSOREXPANSIONMANAGER_H
#define SENSOREXPANSIONMANAGER_H

#include <Arduino.h>
#include <Adafruit_INA219.h>
#include <Adafruit_MAX31855.h>
#include <ADS1256.h>
#include <HX711.h>
#include "CommunicationManager.h"
#include "PackageManager.h"
#include "PackageTypes.h"
#include "ReportManager.h"
#include "consts.h"


class SensorExpansionManager{
public:

    // SensorExpansionManager() 
    //   : buffer(BUFFER_MAX_SIZE, PACKAGE_SIZE, &myTransfer), 
    //     ads1256(ADS1256_DRDY, ADS1256_RST, ADS1256_PDWN, ADS1256_CS, 2.500),
    //     hx711(),
    //     max31855(MAX31855_SCK_PIN, MAX31855_CS_PIN, MAX31855_MISO_PIN),
    //     ina219() {}

    SensorExpansionManager();

    // Init
    void init();
    bool initializeADS1256();
    bool initializeHX711();
    bool initializeMAX31855();
    bool initializeINA();

    // Get Data
    void getADS1256data();
    void getHX711data();
    void getMAX31855data();
    void getINAdata();

    // Update Data
    void updateADS1256data(int id);
    void updateHX711data(int id);
    void updateMAX31855data(int id);
    void updateINAdata(int id);
    

    // Data Rate
    unsigned long getRate4Interval();
    void setRate4Interval(unsigned long interval);
    unsigned long getRate10Interval();
    void setRate10Interval(unsigned long interval);
    unsigned long getRate80Interval();
    void setRate80Interval(unsigned long interval);
    unsigned long getRate100Interval();
    void setRate100Interval(unsigned long interval);
    unsigned long getRate1000Interval();
    void setRate1000Interval(unsigned long interval);
    

private:

    SerialTransfer myTransfer;
    PackageManager buffer;
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
    float_t _max31855data;
    float_t _thermistordata;
    uint16_t _inadata;
    uint32_t _ads1256data;

    // bool _reading_ADS;
    // bool _reading_HX;
    // bool _reading_INA;
    // bool _reading_MAX;

    // unsigned long _updateInterval_ADS;
    // unsigned long _updateInterval_HX;
    // unsigned long _updateInterval_INA;
    // unsigned long _updateInterval_MAX;

    unsigned long _rate4Interval;
    unsigned long _rate10Interval;
    unsigned long _rate80Interval;
    unsigned long _rate100Interval;
    unsigned long _rate1000Interval;

};

#endif