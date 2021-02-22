#include "Devices/EEPROM_TC.h"

//  class variables
EEPROM_TC* EEPROM_TC::_instance = nullptr;

//  class methods
/**
 * accessor for singleton
 */
EEPROM_TC* EEPROM_TC::instance() {
  if (!_instance) {
    _instance = new EEPROM_TC();
  }
  return _instance;
}

//  instance methods
/**
 * Constructor sets pins, dimensions, and shows splash screen
 */
EEPROM_TC::EEPROM_TC() {
}

// getter methods
double EEPROM_TC::getPH() {
  return EEPROM.read(PH_ADDRESS);
}
double EEPROM_TC::getTemp() {
  return EEPROM.read(TEMP_ADDRESS);
}
double EEPROM_TC::getTankID() {
  return EEPROM.read(TANK_ID_ADDRESS);
}
double EEPROM_TC::getCorrectedTemp() {
  return EEPROM.read(TEMP_CORR_ADDRESS);
}
double EEPROM_TC::getKP() {
  return EEPROM.read(KP_ADDRESS);
}
double EEPROM_TC::getKI() {
  return EEPROM.read(KI_ADDRESS);
}
double EEPROM_TC::getKD() {
  return EEPROM.read(KD_ADDRESS);
}
double EEPROM_TC::getMac() {  // See issue #57 about this function
  return EEPROM.read(MAC_ADDRESS);
}
double EEPROM_TC::getHeat() {
  return EEPROM.read(HEAT_ADDRESS);
}
double EEPROM_TC::getAmplitude() {
  return EEPROM.read(AMPLITUDE_ADDRESS);
}
double EEPROM_TC::getFrequency() {
  return EEPROM.read(FREQUENCY_ADDRESS);
}
double EEPROM_TC::getGranularity() {
  return EEPROM.read(GRANULARITY_ADDRESS);
}
double EEPROM_TC::getMaxDataAge() {
  return EEPROM.read(MAX_DATA_AGE_ADDRESS);
}
double EEPROM_TC::getPHSeriesSize() {
  return EEPROM.read(PH_SERIES_SIZE_ADDRESS);
}
double EEPROM_TC::getPHSeriesPointer() {
  return EEPROM.read(PH_SERIES_POINTER_ADDRESS);
}
double EEPROM_TC::getTempSeriesSize() {
  return EEPROM.read(TEMP_SERIES_SIZE_ADDRESS);
}
double EEPROM_TC::getTempSeriesPointer() {
  return EEPROM.read(TEMP_SERIES_POINTER_ADDRESS);
}
double EEPROM_TC::getPHInterval() {
  return EEPROM.read(PH_INTERVAL_ADDRESS);
}
double EEPROM_TC::getPHDelay() {
  return EEPROM.read(PH_DELAY_ADDRESS);
}
double EEPROM_TC::getTempInterval() {
  return EEPROM.read(TEMP_INTERVAL_ADDRESS);
}
double EEPROM_TC::getTempDelay() {
  return EEPROM.read(TEMP_DELAY_ADDRESS);
}

// setter methods
void EEPROM_TC::setPH(double value) {
  EEPROM.update(PH_ADDRESS, value);
}
void EEPROM_TC::setTemp(double value) {
  EEPROM.update(TEMP_ADDRESS, value);
}
void EEPROM_TC::setTankID(double value) {
  EEPROM.update(TANK_ID_ADDRESS, value);
}
void EEPROM_TC::setCorrectedTemp(double value) {
  EEPROM.update(TEMP_CORR_ADDRESS, value);
}
void EEPROM_TC::setKP(double value) {
  EEPROM.update(KP_ADDRESS, value);
}
void EEPROM_TC::setKI(double value) {
  EEPROM.update(KI_ADDRESS, value);
}
void EEPROM_TC::setKD(double value) {
  EEPROM.update(KD_ADDRESS, value);
}
void EEPROM_TC::setMac(double value) {
  EEPROM.update(MAC_ADDRESS, value);
}  // Not used?
void EEPROM_TC::setHeat(double value) {
  EEPROM.update(HEAT_ADDRESS, value);
}
void EEPROM_TC::setAmplitude(double value) {
  EEPROM.update(AMPLITUDE_ADDRESS, value);
}
void EEPROM_TC::setFrequency(double value) {
  EEPROM.update(FREQUENCY_ADDRESS, value);
}
void EEPROM_TC::setGranularity(double value) {
  EEPROM.update(GRANULARITY_ADDRESS, value);
}
void EEPROM_TC::setMaxDataAge(double value) {
  EEPROM.update(MAX_DATA_AGE_ADDRESS, value);
}
void EEPROM_TC::setPHSeriesSize(double value) {
  EEPROM.update(PH_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC::setPHSeriesPointer(double value) {
  EEPROM.update(PH_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC::setTempSeriesSize(double value) {
  EEPROM.update(TEMP_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC::setTempSeriesPointer(double value) {
  EEPROM.update(TEMP_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC::setPHInterval(double value) {
  EEPROM.update(PH_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setPHDelay(double value) {
  EEPROM.update(PH_DELAY_ADDRESS, value);
}
void EEPROM_TC::setTempInterval(double value) {
  EEPROM.update(TEMP_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setTempDelay(double value) {
  EEPROM.update(TEMP_DELAY_ADDRESS, value);
}
