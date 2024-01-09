#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H


#include <Arduino.h>
#include <iostream>
#include "Wire.h"
#include "SerialTransfer.h"

class CommunicationManager{

public:
    CommunicationManager(const std::string& portName, unsigned long baudRate): portName_(portName), baudRate_(baudRate){

    };

    ~CommunicationManager(){
        close();
    };

    bool open(){

        if (Serial){
            return false;
        }

        Serial.begin(baudRate_);
        Serial.setTimeout(1000);

        if (!Serial){
            return false;
        }

        return true;
    }

    void close(){
        Serial.end();
    }

    template <typename T>
    void sendData(const T& data){
        myTransfer_.sendDatum(data);
    }

private:

    std::string portName_;
    const unsigned long baudRate_;
    SerialTransfer myTransfer_;

};

#endif