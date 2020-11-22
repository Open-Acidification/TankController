#include <Arduino.h>

// class temp control -temp setpoint +get/set temp target +virtual updateCtrl(),  subclass heater (respective code) and
// chiller (respective code)

// Temperature Control
// Controlling the tank temperature is done by external devices that are controlled by setting designated pins on the Arduino.
// 
class TempControl_TC {
  private:
    const int CHILLER_PIN = 47;
    const unsigned long CHILLER_INTERVAL = 30 * 1000UL;  // interval at which to change chiller state

    unsigned long currentTime = millis();
    unsigned long previousTime = 0;
    double tempTarget;

  public:
    TempControl_TC() {
    }

    void setTempTarget();
    double getTempTarget() { return tempTarget; }
    virtual void updateCtrl();
};

class Heater_TC : public TempControl_TC {
private:
public:
  void heat();
};

class Chiller_TC : public TempControl_TC {
private:
public:
  void chill();
};
