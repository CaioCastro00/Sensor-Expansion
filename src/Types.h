#include "Arduino.h"

enum SensorID : uint8_t
{
    INIT,
    ADS1256_PT_01,
    ADS1256_PT_T1,
    ADS1256_PT_T2,
    ADS1256_PT_C1,
    ADS1256_PT_N1,
    HX711_WT_01,
    HX711_WT_T1,
    ADS1256_WT_C1,
    MAX31855_TT_O1,
    MAX31855_TT_T1,
    MAX31855_TT_C1,
    MAX31855_TT_C2,
    MAX31855_TT_C3,
    MAX31855_TT_C4,
    THERMISTOR_TR_T1,
    THERMISTOR_TR_C1,
    VALVE_STATE_SPV_T1,
    VALVE_STATE_SPV_O1,
    VALVE_STATE_SPV_M1,
    INA219_VOLTAGE_DL,
    INA219_CURRENT_DL,
    INA219_VOLTAGE_DHE,
    INA219_CURRENT_DHE
};

struct Package
{
    uint8_t datagram_ID;   //     // Define type of next payload data // 1 - unsigned char
    uint32_t timestamp;    // 4 - unsigned long
} __attribute__((packed)); // attribute packed: Set smallest possible alignment.

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

struct FloatPayload : Package
{
    float_t value;

    FloatPayload(uint8_t id, uint32_t ts, float val) : Package{id, ts}, value(val) {}
} __attribute__((packed));

struct Uint8Payload : Package
{
    uint8_t value;
} __attribute__((packed));

struct Uint16Payload : Package
{
    uint16_t value;
} __attribute__((packed));

struct Uint32Payload : Package
{
    uint32_t value;
} __attribute__((packed));

struct Uint64Payload : Package
{
    uint64_t value;
} __attribute__((packed));

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