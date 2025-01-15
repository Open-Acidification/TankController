#include <Arduino.h>

#include "TankController.h"
#include "wrappers/Serial_TC.h"

class PHControl {
public:
  enum pHFunctionTypes {
    FLAT_TYPE,
    RAMP_TYPE,
    SINE_TYPE,
  };

private:
  // Class variable
  static PHControl *_instance;
  // instance variables
  uint32_t lastSwitchMS = 0;
  uint32_t lastWarnMS = 0;
  const uint16_t PH_CONTROL_PIN = 49;
  const uint16_t SOLENOID_OPENING_TIME = 100;
  float baseTargetPh;     // base target
  float currentTargetPh;  // current target (ramp, sine, arbitrary)
  float rampInitialValue;
  uint32_t rampTimeStartSeconds;
  uint32_t rampTimeEndSeconds;
  float amplitude;
  uint32_t periodInSeconds;
  uint32_t sineStartTime;
  const uint16_t WINDOW_SIZE = 10000;  // 10 second Proportional output window (for PID)
  bool usePID = true;
  pHFunctionTypes pHFunctionType = FLAT_TYPE;
  PHControl();

public:
  static PHControl *instance();
  static void clearInstance();
  float getBaseTargetPh() {
    return baseTargetPh;
  }
  float getCurrentTargetPh() {
    return currentTargetPh;
  }
  pHFunctionTypes getPHFunctionType() {
    return pHFunctionType;
  }
  float getAmplitude() {
    return amplitude;
  }
  uint32_t getPhRampTimeStart() {
    return pHFunctionType == FLAT_TYPE ? 0 : rampTimeStartSeconds;
  }
  uint32_t getPhRampTimeEnd() {
    return pHFunctionType == FLAT_TYPE ? 0 : rampTimeEndSeconds;
  }
  uint32_t getPeriodInSeconds() {
    return periodInSeconds;
  }
  bool getUsePID() {
    return usePID;
  }
  bool isOn();
  void setBaseTargetPh(float newPh);
  void setRampDurationHours(float newPhRampDuration);
  void setSineAmplitude(float sineAmplitude);
  void setSineAmplitudeAndHours(float sineAmplitude, float sinePeriodInHours);
  void enablePID(bool flag);
  void loop();
};
