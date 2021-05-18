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
  float getPh() {
    return value;
  }
  String getSlopeResponse() {
    return slopeResponse;
  }
  void clearCalibration();
  String getSlope();
  void sendSlopeRequest();
  void serialEvent1();
  void setHighpointCalibration(float highpoint);
  void setLowpointCalibration(float lowpoint);
  void setMidpointCalibration(float midpoint);
  void setTemperatureCompensation(float temperature);

private:
  // Class variable
  static PHProbe* _instance;
  // instance variable
  float value = 0;
  String slopeResponse = "";
  // Methods
  PHProbe();
};
