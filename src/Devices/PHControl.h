#include <Arduino.h>

#include "TankController.h"

class PHControl {
private:
  // Class variable
  static PHControl *_instance;
  // instance variables
  uint32_t lastSwitchMS = 0;
  uint32_t lastWarnMS = 0;
  const uint16_t PH_CONTROL_PIN = 49;
  const uint16_t SOLENOID_OPENING_TIME = 100;
  float targetPh; // actual target
  float currentPHTarget;
  float rampStartingPh;
  uint32_t rampTimeStart;
  uint32_t rampTimeEnd;
  float amplitude;
  uint32_t period;
  uint32_t sineStartTime;
  const uint16_t WINDOW_SIZE =
      10000; // 10 second Proportional output window (for PID)
  bool usePID = true;
  int pHSetType = FLAT_TYPE;
  PHControl();

public:
  enum phSetTypeTypes {
    FLAT_TYPE,
    RAMP_TYPE,
    SINE_TYPE,
  };
  static PHControl *instance();
  static void clearInstance();
  float getTargetPh() { return targetPh; }
  float getCurrentPhTarget() { return currentPHTarget; }
  int getPhSetType() { return pHSetType; }
  float getAmplitude() { return amplitude; }
  uint32_t getPhRampTimeStart() { return rampTimeStart; }
  uint32_t getPhRampTimeEnd() { return rampTimeEnd; }
  uint32_t getPeriod() { return period; }
  bool getUsePID() { return usePID; }
  bool isOn();
  void setTargetPh(float newPh);
  void setRampDuration(float newPhRampDuration);
  void setSine(float sineAmplitude, float sinePeriodInHours);
  void enablePID(bool flag);
  void updateControl(float pH);
};
