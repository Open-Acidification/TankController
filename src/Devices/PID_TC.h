#include <Arduino.h>
#include <PID_v1.h>

class PID_TC {
public:
  // declare singleton
  static PID_TC *instance();
  // instance functions
  double computeOutput(double target, double current);
  void logToSerial();
  void setTunings(double Kp, double Ki, double Kd);
  // used by tests to get Kp, Ki, Kd
  PID *getPID() {
    return pPID;
  }

private:
  // class
  static PID_TC *_instance;
  // declare constructor
  PID_TC();
  // declare necessary private variables
  double input = 0.0;
  double output = 0.0;
  double set_point = 0.0;
  const int WINDOW_SIZE = 10000;
  PID *pPID;
};
