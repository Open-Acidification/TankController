#pragma once
#include "Arduino.h"

/**
 * wrapper class for EZO pH Circuit | Atlas Scientific
 * Issuing the "Cal,mid,n\r" command will
 * clear the other calibration points.
 */

// getPH() function for testing purposes
class PHProbe {
public:
  static PHProbe* instance();
  double getPH() {
    return value;
  }
  String getSlopeResponse() {
    return slopeResponse;
  }
  void clearCalibration();
  double getPhReading();
  void onePointCalibration(double midpoint);
  String getSlope();
  void serialEvent1();
  void setTemperatureCompensation(double temperature);
  void twoPointCalibration(double lowpoint, double midpoint);

private:
  // Class variable
  static PHProbe* _instance;
  // instance variable
  double value = 0;
  String slope = "";
  String slopeResponse = "";
  // Methods
  PHProbe();
};
