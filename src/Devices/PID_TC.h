#include <Arduino.h>
#include <PID_v1.h>

class PID_TC {
public:
  // declare singleton
  static PID_TC *instance();
  static void reset();  // used by tests
  // instance functions
  float computeOutput(float target, float current);
  float getKd() {
    return pPID->GetKd();
  }
  float getKi() {
    return pPID->GetKi();
  }
  float getKp() {
    return pPID->GetKp();
  }
  int getMode() {
    return pPID->GetMode();
  }
  void logToSerial();
  void setKd(float Kd);
  void setKi(float Ki);
  void setKp(float Kp);
  void setTunings(float Kp, float Ki, float Kd);

private:
  // class
  static PID_TC *_instance;
  // declare constructor & destructor
  PID_TC();
  ~PID_TC();
  // declare necessary private variables
  // note that on the Mega2560 the `double` type is 4 bytes!
  double input = 0.0;
  double output = 0.0;
  double set_point = 0.0;
  const int WINDOW_SIZE = 10000;
  PID *pPID;
};
