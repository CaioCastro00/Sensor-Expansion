#ifndef PACKAGE_TYPES_H
#define PACKAGE_TYPES_H

#include "Arduino.h"

#define TRANSMISSION_RATE_HZ            4
#define PACKAGE_SIZE                    9

//General buffer: Buffer will be empty every 1/4s
#define BUFFER_MAX_SIZE                 PACKAGE_SIZE * 1020 //4044 * (1/4)s = 1011 ~ 1020 (Considering snario )

//Payload buffer
#define PAYLOAD_BUFFER_MAX_PACKAGES     28 //Case limit a 254B of SerialTransfer
#define PAYLOAD_BUFFER_MAX_SIZE         PACKAGE_SIZE * PAYLOAD_BUFFER_MAX_PACKAGES

enum Datagram : uint8_t
{
    INIT,

    // 1 a 14 - uint8_t(1B)
    VALVE_STATE_SPV_T1, // 1B
    VALVE_STATE_SPV_O1,
    VALVE_STATE_SPV_M1,

    // 15 a 24 - uint32_t(4B)
    ADS1256_PT_01 = 15,
    ADS1256_PT_T1,
    ADS1256_PT_T2,
    ADS1256_PT_C1,
    ADS1256_PT_N1,
    ADS1256_WT_C1,

    // 25 a 44 - Float_t(4B)
    HX711_WT_01 = 25,
    HX711_WT_T1,
    MAX31855_TT_O1,
    MAX31855_TT_T1,
    MAX31855_TT_C1,
    MAX31855_TT_C2,
    MAX31855_TT_C3,
    MAX31855_TT_C4,
    THERMISTOR_TR_T1,
    THERMISTOR_TR_C1,
    INA219_VOLTAGE_DL,
    INA219_CURRENT_DL,
    INA219_VOLTAGE_DHE,
    INA219_CURRENT_DHE
};


struct Package
{
    Datagram datagramID;   //     // Define type of next payload data // 1 - unsigned char
    uint32_t timestamp;    // 4 - unsigned long
    // float_t value;
} __attribute__((packed)); // attribute packed: Set smallest possible alignment.

struct FloatPayload : Package
{
    float_t value;
} __attribute__((packed));

struct Uint8Payload : Package
{
    uint8_t value;
    uint8_t virtualPadding[3];
    // } __attribute__((packed, aligned(X)));
} __attribute__((packed));

template <class T>
struct PackageBuffer
{
    uint16_t bufferSize; // 2B
    uint8_t errorCode;   // 1B
    T buffer[27];  // 27*9B
} __attribute__((packed));

// union PayloadUnion {
//     FloatPayload floatPayload;
//     Uint8Payload uint8Payload;
// };

/*
|     timestamp     | 4B
|  id |    padd     | 1B + 3B -> Normal alligment
|      value        | 4B
*/

/*
|     timestamp     | padd | 4B + 1B -> Possible normal alligment
|  id  |       value       | 5B
*/

/*
|     timestamp     |  id  |       value       | 9B -> Smallest possible alligment
*/
// https://www.geeksforgeeks.org/structure-member-alignment-padding-and-data-packing/


struct Uint16Payload : Package
{
    uint16_t value;
    uint8_t virtualPadding[2];
} __attribute__((packed));

struct Uint32Payload : Package
{
    uint32_t value;
} __attribute__((packed));

struct Uint64Payload : Package
{
    uint64_t value;
} __attribute__((packed));

// typedef struct PackageBuffer<T> PackageBuffer;
typedef FloatPayload MAX31855Packet;
typedef FloatPayload ThermistorPackage;

// class MAX31855Wrapper
// {
// private:
//     Package lastPackage[2] = {Uint32Payload{}, Uint64Payload{}};
//     // MAX31855Packet lastPackage;
//     /* data */
// public:
//     MAX31855Packet getLastPackage();
// };

#endif