#include <Arduino.h>

#include "TankController.h"

class PHControl {
private:
  // Class variable
  static PHControl* _instance;
  // instance variables
  uint32_t lastSwitchMS = 0;
  uint32_t lastWarnMS = 0;
  const uint16_t PH_CONTROL_PIN = 49;
  const uint16_t SOLENOID_OPENING_TIME = 100;
  float targetPh;
  float currentPHTarget;
  float rampStartingPh;
  uint32_t rampTimeStart;
  uint32_t rampTimeEnd;
  const uint16_t WINDOW_SIZE = 10000;  // 10 second Proportional output window (for PID)
  bool usePID = true;
  PHControl();

public:
  static PHControl* instance();
  static void clearInstance();
  float getTargetPh() {
    return targetPh;
  }
  float getCurrentPHTarget() {
    return currentPHTarget;
  }
  uint32_t getRampTimeStart() {
    return rampTimeStart;
  }
  uint32_t getRampTimeEnd() {
    return rampTimeEnd;
  }
  bool getUsePID() {
    return usePID;
  }
  bool isOn();
  void setTargetPh(float newPh);
  void setRamp(float newPhRampTime);
  void enablePID(bool flag);
  void updateControl(float pH);
};
