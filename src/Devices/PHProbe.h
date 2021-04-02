#pragma once
#include "Arduino.h"

/**
 * wrapper class for EZO pH Circuit | Atlas Scientific
 * Issuing the "Cal,mid,n\r" command will
 * clear the other calibration points.
 */

// getValue() function is for testing purposes
class PHProbe {
public:
  static PHProbe* instance();
  double getPh() {
    return value;
  }
  String getSlopeResponse() {
    return slopeResponse;
  }
  void clearCalibration();
  void onePointCalibration(double midpoint);
  String getSlope();
  void sendSlopeRequest();
  void serialEvent1();
  void setTemperatureCompensation(double temperature);
  void twoPointCalibration(double lowpoint, double midpoint);

private:
  // Class variable
  static PHProbe* _instance;
  // instance variable
  double value = 0;
  String slopeResponse = "";
  // Methods
  PHProbe();
};
