#pragma once
#include <Arduino.h>

/**
 * wrapper class for EZO pH Circuit | Atlas Scientific
 *
 * https://www.atlas-scientific.com/files/pH_EZO_Datasheet.pdf
 *
 * Issuing the "Cal,mid,n\r" command will
 * clear the other calibration points.
 *
 * While the data sheet uses "Slope" the actual string is "SLOPE"
 * Similarly, "Cal,?" is actually "CAL,?" and responses are "?CAL,2" for example.
 */

// getValue() function is for testing purposes
class PHProbe {
public:
  static PHProbe* instance();
  float getPh() {
    return value;
  }
  const char* getSlopeResponse() const {
    return slopeResponse;
  }
  void clearCalibration();
  void getCalibration(char* buffer, int size);
  void getSlope(char* buffer, int size);
  void sendCalibrationRequest();
  void sendSlopeRequest();
  void serialEvent1();
  void setHighpointCalibration(float highpoint);
  void setLowpointCalibration(float lowpoint);
  void setMidpointCalibration(float midpoint);
  void setTemperatureCompensation(float temperature);
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  void setCalibrationPoints(int newValue);
  void setPh(float newValue);
  void setPhSlope(const char* slope = "?SLOPE,99.7,100.3,-0.89\r");
#endif
private:
  // Class variable
  static PHProbe* _instance;
  // instance variable
  float value = 0;
  char calibrationResponse[14] = "";
  char slopeResponse[32] = "";
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  int calibrationPoints = 0;
#endif
  // Methods
  PHProbe();
};
