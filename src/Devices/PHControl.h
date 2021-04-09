#include <Arduino.h>

class PHControl {
private:
  const int PIN = 49;
  const int SOLENOID_OPENING_TIME = 100;
  double targetPh;
  const int WINDOW_SIZE = 10000;  // 10 second Time Proportional output window
  long onTime = 0;
  long window_start_time;
  bool usePID = true;
public:
  PHControl();
  const int getPIN() {
    return PIN;
  }
  void setTargetPh(double newPh) {
    targetPh = newPh;
  }
  void setUsePID(bool flag) {
    usePID = flag;
  }
  void updateControl(double pH);
};