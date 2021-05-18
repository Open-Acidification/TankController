#pragma once

#include <Arduino.h>

class EEPROM_TC {
public:
  // class methods
  // return singleton (the current version by default)
  static EEPROM_TC* instance(int version = 0);

  // destructor
  virtual ~EEPROM_TC() {
  }

  // accessor methods
  virtual float getAmplitude() = 0;
  virtual float getCorrectedTemp() = 0;
  virtual float getFrequency() = 0;
  virtual int getGoogleSheetInterval() = 0;
  virtual float getGranularity() = 0;
  virtual bool getHeat() = 0;
  virtual float getKD() = 0;
  virtual float getKI() = 0;
  virtual float getKP() = 0;
  virtual float getMac() = 0;
  virtual float getMaxDataAge() = 0;
  virtual float getPH() = 0;
  virtual float getPHDelay() = 0;
  virtual float getPHInterval() = 0;
  virtual float getPHSeriesPointer() = 0;
  virtual float getPHSeriesSize() = 0;
  virtual int getTankID() = 0;
  virtual float getTemp() = 0;
  virtual float getTempDelay() = 0;
  virtual float getTempInterval() = 0;
  virtual float getTempSeriesPointer() = 0;
  virtual float getTempSeriesSize() = 0;
  virtual int getVersion() = 0;
  // setter methods
  virtual void setAmplitude(float value) = 0;
  virtual void setCorrectedTemp(float value) = 0;
  virtual void setFrequency(float value) = 0;
  virtual void setGoogleSheetInterval(int value) = 0;
  virtual void setGranularity(float value) = 0;
  virtual void setHeat(bool value) = 0;
  virtual void setKD(float value) = 0;
  virtual void setKI(float value) = 0;
  virtual void setKP(float value) = 0;
  virtual void setMac(float value) = 0;
  virtual void setMaxDataAge(float value) = 0;
  virtual void setPH(float value) = 0;
  virtual void setPHDelay(float value) = 0;
  virtual void setPHInterval(float value) = 0;
  virtual void setPHSeriesPointer(float value) = 0;
  virtual void setPHSeriesSize(float value) = 0;
  virtual void setTankID(int value) = 0;
  virtual void setTemp(float value) = 0;
  virtual void setTempDelay(float value) = 0;
  virtual void setTempInterval(float value) = 0;
  virtual void setTempSeriesPointer(float value) = 0;
  virtual void setTempSeriesSize(float value) = 0;
  virtual void setVersion() = 0;
  // read and write
  float eepromReadFloat(int address);
  int eepromReadInt(int address);
  void eepromWriteFloat(int address, float value);
  void eepromWriteInt(int address, int value);

protected:
  virtual bool isRightVersion() = 0;

private:
  // class variables
  static EEPROM_TC* _instance;
};
