#include <Arduino.h>

// class temp control -temp setpoint +get/set temp target +virtual updateCtrl(),  subclass heater (respective code) and
// chiller (respective code)

// Temperature Control
// Controlling the tank temperature is done by external devices that are controlled by setting designated pins on the
// Arduino.
//
class TemperatureControl {
protected:
  const int PIN = 47;
  const double DELTA = 0.05;
  double targetTemperature;
  bool currentSwitchState = false;

  void turnOn(bool state);

public:
  TemperatureControl() {
  }
  void setTargetTemperature(double newTemperature) {
    targetTemperature = newTemperature;
  }
  virtual void updateControl(double currentTemperature) = 0;
  bool getCurrentSwitchState() {
    return currentSwitchState;
  }
};

class Heater : public TemperatureControl {
private:
public:
  void updateControl(double currentTemperature);
};

class Chiller : public TemperatureControl {
private:
  const unsigned long TIME_INTERVAL = 30 * 1000UL;  // interval at which to change chiller state
  unsigned long previousMillis = 0;                 // will store last time chiller state was checked

public:
  void updateControl(double currentTemperature);
};

