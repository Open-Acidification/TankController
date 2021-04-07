#include <Arduino.h>

class PHControl {
private:
  const int PIN = 49;
  double targetPh;
  const int WINDOW_SIZE = 10000;  // 10 second Time Proportional output window
  long window_start_time;
  bool usePID = true;
public:
  PHControl() {
    window_start_time = millis();
  }
  void setTargetPh(double newPh) {
    targetPh = newPh;
  }
  void setUsePID(bool flag) {
    usePID = flag;
  }
  void updateControl(double pH);
  bool getCurrentSwitchState() {
    return currentSwitchState;
  }
};