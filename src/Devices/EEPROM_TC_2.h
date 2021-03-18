#pragma once

#include "Devices/EEPROM_TC.h"

class EEPROM_TC_2 : public EEPROM_TC {
public:
  // accessor methods
  int getVersion();
  double getPH();
  double getTemp();
  int getTankID();
  double getCorrectedTemp();
  double getKP();
  double getKI();
  double getKD();
  double getMac();  // See issue #57 about this function
  bool getHeat();
  double getAmplitude();
  double getFrequency();
  double getGranularity();
  double getMaxDataAge();
  double getPHSeriesSize();
  double getPHSeriesPointer();
  double getTempSeriesSize();
  double getTempSeriesPointer();
  double getPHInterval();
  double getPHDelay();
  double getTempInterval();
  double getTempDelay();

  // setter methods
  void setVersion() {
  }  // Nothing to be done here!
  void setPH(double value);
  void setTemp(double value);
  void setTankID(int value);
  void setCorrectedTemp(double value);
  void setKP(double value);
  void setKI(double value);
  void setKD(double value);
  void setMac(double value);
  void setHeat(bool value);
  void setAmplitude(double value);
  void setFrequency(double value);
  void setGranularity(double value);
  void setMaxDataAge(double value);
  void setPHSeriesSize(double value);
  void setPHSeriesPointer(double value);
  void setTempSeriesSize(double value);
  void setTempSeriesPointer(double value);
  void setPHInterval(double value);
  void setPHDelay(double value);
  void setTempInterval(double value);
  void setTempDelay(double value);

protected:
  virtual bool isRightVersion();

private:
  // NOTE BUG: many addresses provide for 4 bytes for a double
  // this is wrong and is fixed in EEPROM_TC_3

  // instance variables from v0.197
  const int PH_ADDRESS = 0;          // 9.999
  const int TEMP_ADDRESS = 4;        // 99.99
  const int TANK_ID_ADDRESS = 8;     // 999
  const int TEMP_CORR_ADDRESS = 12;  // 99.99
  const int KP_ADDRESS = 20;         // double
  const int KI_ADDRESS = 28;         // double
  const int KD_ADDRESS = 36;         // double
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
};
