#include "PackageTypes.h"
#include "SensorExpansionManager.h"

SensorExpansionManager::SensorExpansionManager(): buffer(BUFFER_MAX_SIZE, PACKAGE_SIZE, &myTransfer), max31855(MAX31855_SCK_PIN, MAX31855_CS_PIN, MAX31855_MISO_PIN), ads1256(ADS1256_DRDY, ADS1256_RST, ADS1256_PDWN, ADS1256_CS, 2.500){ //, ads1256(ADS1256_DRDY, ADS1256_RST, ADS1256_PDWN, ADS1256_CS, 2.500) 

}

void SensorExpansionManager::init(){
        initializeADS1256();
        initializeHX711();
        initializeMAX31855();
        initializeINA();
    }

bool SensorExpansionManager::initializeADS1256(){
    ads1256.setupADC(ADS1256_DRDY, ADS1256_RST, ADS1256_PDWN, ADS1256_CS, 2.500); //DRDY, RESET, SYNC(PDWN), CS, VREF(float).
    ads1256.InitializeADC();
    ads1256.setDRATE(DRATE_7500SPS);
    return true;
}

bool SensorExpansionManager::initializeHX711(){
    hx711.begin(HX711_DOUT_PIN, HX711_SCK_PIN);
    hx711.set_scale(CALIBRATION_FACTOR);
    hx711.tare();
    return true;
}

bool SensorExpansionManager::initializeMAX31855(){
    max31855.setup(MAX31855_SCK_PIN, MAX31855_CS_PIN, MAX31855_MISO_PIN); // int8_t _sclk, int8_t _cs, int8_t _miso
    return max31855.begin();
}

bool SensorExpansionManager::initializeINA(){
    if (!ina219.begin()){
        return false;
    }
    return true;
}

void SensorExpansionManager::getSingleADS1256data(){

    for (int i = 0; i < SINGLE_PORTS; i++)
    {
        _ads1256data = ads1256.convertToVoltage(ads1256.cycleSingle());
        updateADS1256data(i+15);
    }
    
    ads1256.stopConversion();
}

void SensorExpansionManager::getDiffADS1256data(){

    for (int i = 0; i < DIFF_PORTS; i++)
    {
        _ads1256data = ads1256.convertToVoltage(ads1256.cycleDifferential());
        updateADS1256data(i+20);
    }
    
    ads1256.stopConversion();
}

void SensorExpansionManager::updateADS1256data(int id){
    ads1256package.datagramID =  (Datagram) id;
    ads1256package.timestamp = millis();
    ads1256package.value = _ads1256data;
    buffer.addItem(ads1256package);
}

void SensorExpansionManager::getHX711data(){
    _hx711data = hx711.get_units();
}

void SensorExpansionManager::updateHX711data(int id){
    hx711package.datagramID =  (Datagram) id;
    hx711package.timestamp = millis();
    hx711package.value = _hx711data;
    buffer.addItem(hx711package);
}

void SensorExpansionManager::getMAX31855data(){
    _max31855data = max31855.readCelsius();         
}

void SensorExpansionManager::updateMAX31855data(int id){
    max31855package.datagramID =  (Datagram) id;
    max31855package.timestamp = millis();
    max31855package.value = _max31855data;
    buffer.addItem(max31855package);
}

void SensorExpansionManager::getINAdata(){
    _inadata = (static_cast<uint8_t>(ina219.getCurrent_mA()) << 8 ) | (static_cast<uint8_t>(ina219.getBusVoltage_V() + (ina219.getShuntVoltage_mV()) / 1000));
}

void SensorExpansionManager::updateINAdata(int id){
    inapackage.datagramID =  (Datagram) id;
    inapackage.timestamp = millis();
    inapackage.value = _inadata;
    buffer.addItem(inapackage);
}

unsigned long SensorExpansionManager::getRate4Interval()
{
  return _rate4Interval;
}

void SensorExpansionManager::setRate4Interval(unsigned long interval)
{
  _rate4Interval = interval;
}

unsigned long SensorExpansionManager::getRate10Interval()
{
  return _rate10Interval;
}

void SensorExpansionManager::setRate10Interval(unsigned long interval)
{
  _rate10Interval = interval;
}

unsigned long SensorExpansionManager::getRate80Interval()
{
  return _rate80Interval;
}

void SensorExpansionManager::setRate80Interval(unsigned long interval)
{
  _rate80Interval = interval;
}

unsigned long SensorExpansionManager::getRate100Interval()
{
  return _rate100Interval;
}

void SensorExpansionManager::setRate100Interval(unsigned long interval)
{
  _rate100Interval = interval;
}

unsigned long SensorExpansionManager::getRate1000Interval()
{
  return _rate1000Interval;
}

void SensorExpansionManager::setRate1000Interval(unsigned long interval)
{
  _rate1000Interval = interval;
}

