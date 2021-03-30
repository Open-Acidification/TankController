#pragma once
#include "Arduino.h"

/**
* Issuing the cal,mid command after the
* EZO pH circuit has been calibrated, will
* clear the other calibration points. 
* Full calibration will have to be redone
*/

class PHProbe {
public:
  static PHProbe* instance();
  void onePointCalibration(double midpoint);
  double getPH() {
    return value;
  }
  void serialEvent1();
  void setTemperatureCompensation(double temperature);
  void twoPointCalibration(double lowpoint, double midpoint);

private:
  // Class variable
  static PHProbe* _instance;
  // instance variable
  double value = 0;
  // Methods
  PHProbe();
};
