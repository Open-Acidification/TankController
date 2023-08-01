#pragma once

#include "../TC_util.h"

class EEPROM_TC {
public:
  static EEPROM_TC* instance();

  // read and write
  float eepromReadFloat(uint16_t address);
  int32_t eepromReadInt(uint16_t address);
  void eepromWriteFloat(uint16_t address, float value);
  void eepromWriteInt(uint16_t address, int32_t value);

  // accessor methods
  float getAmplitude();               // not used
  float getCorrectedTemp();           // adjustment to temperature probe
  float getFrequency();               // not used
  uint16_t getGoogleSheetInterval();  // how often to send data using PushingBox
  float getGranularity();             // not used
  bool getHeat();                     // boolean for heater (true) or chiller (false)
  bool getIgnoreBadPHSlope();         // whether to warn about an out-of-range calibration
  float getKD();                      // constants used for PID to calculate bubbler timer
  float getKI();                      // constants used for PID to calculate bubbler timer
  float getKP();                      // constants used for PID to calculate bubbler timer
  void getMac(uint8_t* bytes);        // used for DHCP request for IP address
  float getMaxDataAge();              // not used
  float getPh();                      // target pH
  uint32_t getPhRampTimeStart();
  uint32_t getPhRampTimeEnd();
  float getRampStartingPh();
  uint32_t getPhSineStartTime();
  uint32_t getPhSinePeriod();
  float getPhSineAmplitude();
  uint32_t getTempRampTimeStart();
  uint32_t getTempRampTimeEnd();
  float getRampStartingTemp();
  float getPhDelay();          // not used
  float getPhInterval();       // not used
  float getPhSeriesPointer();  // not used
  float getPhSeriesSize();     // not used
  uint16_t getPhSetType();
  uint16_t getTempSetType();
  uint16_t getTankID();  // unique tank identifier for reporting
  uint32_t getTempSineStartTime();
  uint32_t getTempSinePeriod();
  float getTempSineAmplitude();
  float getTemp();               // target temperature
  float getTempDelay();          // not used
  float getTempInterval();       // not used
  float getTempSeriesPointer();  // not used
  float getTempSeriesSize();     // not used

  // setter methods
  void setAmplitude(float value);
  void setCorrectedTemp(float value);
  void setFrequency(float value);
  void setGoogleSheetInterval(uint16_t value);
  void setGranularity(float value);
  void setHeat(bool value);
  void setIgnoreBadPHSlope(bool value);
  void setKD(float value);
  void setKI(float value);
  void setKP(float value);
  void setMac(uint8_t* bytes);
  void setMaxDataAge(float value);
  void setPh(float value);
  void setPhRampTimeStart(uint32_t value);
  void setPhRampTimeEnd(uint32_t value);
  void setRampStartingPh(float value);
  void setPhSineStartTime(uint32_t value);
  void setPhSinePeriod(uint32_t value);
  void setPhSineAmplitude(float value);
  void setTempRampTimeStart(uint32_t value);
  void setTempRampTimeEnd(uint32_t value);
  void setRampStartingTemp(float value);
  void setPhDelay(float value);
  void setPhInterval(float value);
  void setPhSeriesPointer(float value);
  void setPhSeriesSize(float value);
  void setPhSetType(uint16_t value);
  void setTempSetType(uint16_t value);
  void setTankID(uint16_t value);
  void setTempSineStartTime(uint32_t value);
  void setTempSinePeriod(uint32_t value);
  void setTempSineAmplitude(float value);
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
  const uint16_t RAMP_STARTING_PH_ADDRESS = 112;
  const uint16_t PH_SINE_AMPLITUDE_ADDRESS = 112;
  const uint16_t RAMP_TIME_START_PH_ADDRESS = 116;
  const uint16_t PH_SINE_START_TIME_ADDRESS = 116;
  const uint16_t RAMP_TIME_END_PH_ADDRESS = 120;
  const uint16_t PH_SINE_PERIOD_ADDRESS = 120;
  const uint16_t RAMP_STARTING_TEMP_ADDRESS = 124;
  const uint16_t TEMP_SINE_AMPLITUDE_ADDRESS = 124;
  const uint16_t RAMP_TIME_START_TEMP_ADDRESS = 128;
  const uint16_t TEMP_SINE_START_TIME_ADDRESS = 128;
  const uint16_t RAMP_TIME_END_TEMP_ADDRESS = 132;
  const uint16_t TEMP_SINE_PERIOD_ADDRESS = 132;
  const uint16_t PH_SET_TYPE_ADDRESS = 136;
  const uint16_t TEMP_SET_TYPE_ADDRESS = 140;
  // new with v23.8.1
  const uint16_t IGNORE_BAD_PH_SLOPE_ADDRESS = 144;

  // class variables
  static EEPROM_TC* _instance;
};
