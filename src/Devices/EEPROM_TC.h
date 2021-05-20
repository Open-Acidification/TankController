#pragma once

#include "Devices/EEPROM_TC.h"
#include "TC_util.h"

class EEPROM_TC {
public:
  static EEPROM_TC* instance();

  // read and write
  float eepromReadFloat(uint16_t address);
  int32_t eepromReadInt(uint16_t address);
  void eepromWriteFloat(uint16_t address, float value);
  void eepromWriteInt(uint16_t address, int32_t value);

  // accessor methods
  float getAmplitude();
  float getCorrectedTemp();
  float getFrequency();
  uint16_t getGoogleSheetInterval();
  float getGranularity();
  bool getHeat();
  float getKD();
  float getKI();
  float getKP();
  float getMac();
  float getMaxDataAge();
  float getPH();
  float getPHDelay();
  float getPHInterval();
  float getPHSeriesPointer();
  float getPHSeriesSize();
  uint16_t getTankID();
  float getTemp();
  float getTempDelay();
  float getTempInterval();
  float getTempSeriesPointer();
  float getTempSeriesSize();

  // setter methods
  void setAmplitude(float value);
  void setCorrectedTemp(float value);
  void setFrequency(float value);
  void setGoogleSheetInterval(uint16_t value);
  void setGranularity(float value);
  void setHeat(bool value);
  void setKD(float value);
  void setKI(float value);
  void setKP(float value);
  void setMac(float value);
  void setMaxDataAge(float value);
  void setPH(float value);
  void setPHDelay(float value);
  void setPHInterval(float value);
  void setPHSeriesPointer(float value);
  void setPHSeriesSize(float value);
  void setTankID(uint16_t value);
  void setTemp(float value);
  void setTempDelay(float value);
  void setTempInterval(float value);
  void setTempSeriesPointer(float value);
  void setTempSeriesSize(float value);
  void setVersion() {  // Nothing to be done here!
  }

private:
  // instance variables from v0.197
  const uint16_t PH_ADDRESS = 0;          // 9.999
  const uint16_t TEMP_ADDRESS = 4;        // 99.99
  const uint16_t TANK_ID_ADDRESS = 8;     // 999
  const uint16_t TEMP_CORR_ADDRESS = 12;  // 99.99
  const uint16_t KP_ADDRESS = 20;         // float
  const uint16_t KI_ADDRESS = 28;         // float
  const uint16_t KD_ADDRESS = 36;         // float
  const uint16_t MAC_ADDRESS = 44;        // 8 byte
  const uint16_t HEAT_ADDRESS = 52;       // bool
  // new with v0.2
  const uint16_t AMPLITUDE_ADDRESS = 56;
  const uint16_t FREQUENCY_ADDRESS = 60;
  const uint16_t GRANULARITY_ADDRESS = 64;   // granularity for SD logging interval
  const uint16_t MAX_DATA_AGE_ADDRESS = 68;  // max data age for SD card
  const uint16_t PH_SERIES_SIZE_ADDRESS = 72;
  const uint16_t PH_SERIES_POINTER_ADDRESS = 76;
  const uint16_t TEMP_SERIES_SIZE_ADDRESS = 80;
  const uint16_t TEMP_SERIES_POINTER_ADDRESS = 84;
  const uint16_t PH_INTERVAL_ADDRESS = 88;
  const uint16_t PH_DELAY_ADDRESS = 92;
  const uint16_t TEMP_INTERVAL_ADDRESS = 96;
  const uint16_t TEMP_DELAY_ADDRESS = 100;
  // new with v0.3
  const uint16_t GOOGLE_INTERVAL_ADDRESS = 108;

  // class variables
  static EEPROM_TC* _instance;
};
