#pragma once

#include <Arduino.h>
#include <EEPROM.h>

class EEPROM_TC {
public:
  // class methods
  static EEPROM_TC* instance();

  // accessor methods
  double getPH();
  double getTemp();
  double getTankID();
  double getCorrectedTemp();
  double getKP();
  double getKI();
  double getKD();
  double getMac();  // See issue #57 about this function
  double getHeat();
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
  void setPH(double value);
  void setTemp(double value);
  void setTankID(double value);
  void setCorrectedTemp(double value);
  void setKP(double value);
  void setKI(double value);
  void setKD(double value);
  void setMac(double value);
  void setHeat(double value);
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

private:
  // class variables
  static EEPROM_TC* _instance;

  // instance variables
  const int PH_ADDRESS = 0;
  const int TEMP_ADDRESS = 4;
  const int TANK_ID_ADDRESS = 8;
  const int TEMP_CORR_ADDRESS = 12;
  const int KP_ADDRESS = 20;
  const int KI_ADDRESS = 28;
  const int KD_ADDRESS = 36;
  const int MAC_ADDRESS = 44;
  const int HEAT_ADDRESS = 52;
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

  // instance methods
  EEPROM_TC();
};
