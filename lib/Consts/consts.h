#ifndef consts_h
#define consts_h

//--------------- ADS1256 Pin definition ---------------
#define ADS1256_RST   23  //ADS1256 reset pin
#define ADS1256_DRDY  22  //ADS1256 data ready
#define ADS1256_CS    10  //ADS1256 chip select
#define ADS1256_DIN   11  //ADS1256 data-in pin
#define ADS1256_DOUT  12  //ADS1256 data-out pin
#define ADS1256_SCLK  13  //ADS1256 clock pin

//PDWM -> +3.3V ()
#define ADS1256_PDWN 0


//---------------- HX711 Pin definition ----------------
#define HX711_DOUT_PIN 3
#define HX711_SCK_PIN  2


//-------------- MAX31855 Pin definition --------------
#define MAX31855_SO_PIN   5
#define MAX31855_CS_PIN   6
#define MAX31855_SCK_PIN  7



#endif