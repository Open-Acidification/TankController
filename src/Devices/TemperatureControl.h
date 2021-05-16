#include <Arduino.h>

/**
 * Temperature Control
 * Controlling the tank temperature is done by external devices that are controlled by setting
 * designated pins on the Arduino.
 */

class TemperatureControl {
private:
  static TemperatureControl* _instance;

protected:
  const int PIN = 47;
  const double DELTA = 0.05;
  unsigned long lastSwitchMS = 0;
  double targetTemperature;
  bool pinValue = HIGH;
  TemperatureControl();

public:
  virtual ~TemperatureControl() {
  }
  static TemperatureControl* instance();
  static void enableHeater(bool flag);
  double getTargetTemperature() {
    return targetTemperature;
  }
  virtual bool isHeater();
  void setTargetTemperature(double newTemperature);
  virtual void updateControl(double currentTemperature) = 0;
};

class Heater : public TemperatureControl {
public:
  Heater() : TemperatureControl(){};
  bool isHeater() {
    return true;
  }
  void updateControl(double currentTemperature);
};

class Chiller : public TemperatureControl {
private:
  const unsigned long TIME_INTERVAL = 30 * 1000UL;  // interval at which to change chiller state
  unsigned long previousMillis = 0;                 // will store last time chiller state was checked

public:
  Chiller() : TemperatureControl(){};
  bool isHeater() {
    return false;
  }
  void updateControl(double currentTemperature);
};
