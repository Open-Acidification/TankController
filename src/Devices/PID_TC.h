#include <Arduino.h>
#include <PID_v1.h>

class PID_TC {
public:
  // declare singleton
  static PID_TC *instance();
  static void reset();  // used by tests
  // instance functions
  double computeOutput(double target, double current);
  double getKd() {
    return pPID->GetKd();
  }
  double getKi() {
    return pPID->GetKi();
  }
  double getKp() {
    return pPID->GetKp();
  }
  int getMode() {
    return pPID->GetMode();
  }
  void logToSerial();
  void setKd(double Kd);
  void setKi(double Ki);
  void setKp(double Kp);
  void setTunings(double Kp, double Ki, double Kd);

private:
  // class
  static PID_TC *_instance;
  // declare constructor & destructor
  PID_TC();
  ~PID_TC();
  // declare necessary private variables
  double input = 0.0;
  double output = 0.0;
  double set_point = 0.0;
  const int WINDOW_SIZE = 10000;
  PID *pPID;
};
