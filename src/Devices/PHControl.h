#include <Arduino.h>

class PHControl {
private:
  // Class variable
  static PHControl* _instance;
  // instance variables
  const int PIN = 49;
  const int SOLENOID_OPENING_TIME = 100;
  double targetPh;
  const int WINDOW_SIZE = 10000;  // 10 second Time Proportional output window
  long onTime = 0;
  long window_start_time;
  bool usePID = true;
  PHControl();

public:
  static PHControl* instance();
  double getTargetPh() {
    return targetPh;
  }
  bool getUsePID() {
    return usePID;
  }
  void setTargetPh(double newPh);
  void enablePID(bool flag);
  void updateControl(double pH);
};
