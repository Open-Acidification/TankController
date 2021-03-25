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
  virtual double getAmplitude() = 0;
  virtual double getCorrectedTemp() = 0;
  virtual double getFrequency() = 0;
  virtual int getGoogleSheetInterval() = 0;
  virtual double getGranularity() = 0;
  virtual bool getHeat() = 0;
  virtual double getKD() = 0;
  virtual double getKI() = 0;
  virtual double getKP() = 0;
  virtual double getMac() = 0;
  virtual double getMaxDataAge() = 0;
  virtual double getPH() = 0;
  virtual double getPHDelay() = 0;
  virtual double getPHInterval() = 0;
  virtual double getPHSeriesPointer() = 0;
  virtual double getPHSeriesSize() = 0;
  virtual int getTankID() = 0;
  virtual double getTemp() = 0;
  virtual double getTempDelay() = 0;
  virtual double getTempInterval() = 0;
  virtual double getTempSeriesPointer() = 0;
  virtual double getTempSeriesSize() = 0;
  virtual int getVersion() = 0;
  // setter methods
  virtual void setAmplitude(double value) = 0;
  virtual void setCorrectedTemp(double value) = 0;
  virtual void setFrequency(double value) = 0;
  virtual void setGoogleSheetInterval(int value) = 0;
  virtual void setGranularity(double value) = 0;
  virtual void setHeat(bool value) = 0;
  virtual void setKD(double value) = 0;
  virtual void setKI(double value) = 0;
  virtual void setKP(double value) = 0;
  virtual void setMac(double value) = 0;
  virtual void setMaxDataAge(double value) = 0;
  virtual void setPH(double value) = 0;
  virtual void setPHDelay(double value) = 0;
  virtual void setPHInterval(double value) = 0;
  virtual void setPHSeriesPointer(double value) = 0;
  virtual void setPHSeriesSize(double value) = 0;
  virtual void setTankID(int value) = 0;
  virtual void setTemp(double value) = 0;
  virtual void setTempDelay(double value) = 0;
  virtual void setTempInterval(double value) = 0;
  virtual void setTempSeriesPointer(double value) = 0;
  virtual void setTempSeriesSize(double value) = 0;
  virtual void setVersion() = 0;
  // read and write
  double eepromReadDouble(int address);
  int eepromReadInt(int address);
  void eepromWriteDouble(int address, double value);
  void eepromWriteInt(int address, int value);

protected:
  virtual bool isRightVersion() = 0;

private:
  // class variables
  static EEPROM_TC* _instance;
};
