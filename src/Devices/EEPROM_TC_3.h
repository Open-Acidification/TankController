#pragma once

#include "Devices/EEPROM_TC.h"

class EEPROM_TC_3 : public EEPROM_TC {
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
  void setVersion();

protected:
  virtual bool isRightVersion();

private:
  // new with v0.3
  const int VERSION_ADDRESS = 0;  // 999
  // from v0.197
  // NOTE: addition represents size of previous value
  const int TANK_ID_ADDRESS = VERSION_ADDRESS + 4;  // 999
  const int HEAT_ADDRESS = TANK_ID_ADDRESS + 4;     // bool
  const int PH_ADDRESS = HEAT_ADDRESS + 4;          // 9.999
  const int TEMP_ADDRESS = PH_ADDRESS + 8;          // 99.99
  const int TEMP_CORR_ADDRESS = TEMP_ADDRESS + 8;   // 99.99
  const int KP_ADDRESS = TEMP_CORR_ADDRESS + 8;     // float
  const int KI_ADDRESS = KP_ADDRESS + 8;            // float
  const int KD_ADDRESS = KI_ADDRESS + 8;            // float
  const int MAC_ADDRESS = KD_ADDRESS + 8;           // 8 byte
  // new with v0.2
  const int AMPLITUDE_ADDRESS = MAC_ADDRESS + 8;
  const int FREQUENCY_ADDRESS = AMPLITUDE_ADDRESS + 8;
  const int GRANULARITY_ADDRESS = FREQUENCY_ADDRESS + 8;     // granularity for SD logging interval
  const int MAX_DATA_AGE_ADDRESS = GRANULARITY_ADDRESS + 8;  // max data age for SD card
  const int PH_SERIES_SIZE_ADDRESS = MAX_DATA_AGE_ADDRESS + 8;
  const int PH_SERIES_POINTER_ADDRESS = PH_SERIES_SIZE_ADDRESS + 8;
  const int TEMP_SERIES_SIZE_ADDRESS = PH_SERIES_POINTER_ADDRESS + 8;
  const int TEMP_SERIES_POINTER_ADDRESS = TEMP_SERIES_SIZE_ADDRESS + 8;
  const int PH_INTERVAL_ADDRESS = TEMP_SERIES_POINTER_ADDRESS + 8;
  const int PH_DELAY_ADDRESS = PH_INTERVAL_ADDRESS + 8;
  const int TEMP_INTERVAL_ADDRESS = PH_DELAY_ADDRESS + 8;
  const int TEMP_DELAY_ADDRESS = TEMP_INTERVAL_ADDRESS + 8;
  // new with v0.3
  const int GOOGLE_INTERVAL_ADDRESS = TEMP_DELAY_ADDRESS + 8;
};
