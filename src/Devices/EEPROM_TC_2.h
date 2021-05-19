#pragma once

#include "Devices/EEPROM_TC.h"

class EEPROM_TC_2 : public EEPROM_TC {
public:
  // accessor methods
  float getAmplitude();
  float getCorrectedTemp();
  float getFrequency();
  int getGoogleSheetInterval();
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
  int getTankID();
  float getTemp();
  float getTempDelay();
  float getTempInterval();
  float getTempSeriesPointer();
  float getTempSeriesSize();
  int getVersion();

  // setter methods
  void setAmplitude(float value);
  void setCorrectedTemp(float value);
  void setFrequency(float value);
  void setGoogleSheetInterval(int value);
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
  void setTankID(int value);
  void setTemp(float value);
  void setTempDelay(float value);
  void setTempInterval(float value);
  void setTempSeriesPointer(float value);
  void setTempSeriesSize(float value);
  void setVersion() {  // Nothing to be done here!
  }

protected:
  virtual bool isRightVersion();

private:
  // NOTE BUG: many addresses provide for 4 bytes for a float
  // this is wrong and is fixed in EEPROM_TC_3

  // instance variables from v0.197
  const int PH_ADDRESS = 0;          // 9.999
  const int TEMP_ADDRESS = 4;        // 99.99
  const int TANK_ID_ADDRESS = 8;     // 999
  const int TEMP_CORR_ADDRESS = 12;  // 99.99
  const int KP_ADDRESS = 20;         // float
  const int KI_ADDRESS = 28;         // float
  const int KD_ADDRESS = 36;         // float
  const int MAC_ADDRESS = 44;        // 8 byte
  const int HEAT_ADDRESS = 52;       // bool
  // new with v0.2
  const int AMPLITUDE_ADDRESS = 56;
  const int FREQUENCY_ADDRESS = 60;
  const int GRANULARITY_ADDRESS = 64;   // granularity for SD logging interval
  const int MAX_DATA_AGE_ADDRESS = 68;  // max data age for SD card
  const int PH_SERIES_SIZE_ADDRESS = 72;
  const int PH_SERIES_POINTER_ADDRESS = 76;
  const int TEMP_SERIES_SIZE_ADDRESS = 80;
  const int TEMP_SERIES_POINTER_ADDRESS = 84;
  const int PH_INTERVAL_ADDRESS = 88;
  const int PH_DELAY_ADDRESS = 92;
  const int TEMP_INTERVAL_ADDRESS = 96;
  const int TEMP_DELAY_ADDRESS = 100;
  // new with v0.3
  const int GOOGLE_INTERVAL_ADDRESS = 108;
};
