#include <Arduino.h>

#include "TankControllerLib.h"

class PHControl {
private:
  // Class variable
  static PHControl* _instance;
  // instance variables
  uint32_t lastSwitchMS = 0;
  const uint16_t PH_CONTROL_PIN = 49;
  const uint16_t SOLENOID_OPENING_TIME = 100;
  float targetPh;
  const uint16_t WINDOW_SIZE = 10000;  // 10 second Time Proportional output window
  long onTime = 0;
  long window_start_time;
  bool usePID = true;
  bool pinValue = TURN_SOLENOID_OFF;
  PHControl();

public:
  static PHControl* instance();
  float getTargetPh() {
    return targetPh;
  }
  bool getUsePID() {
    return usePID;
  }
  bool isOn();
  void setTargetPh(float newPh);
  void enablePID(bool flag);
  void updateControl(float pH);
};
