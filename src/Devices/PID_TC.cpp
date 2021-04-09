#include "Devices/PID_TC.h"

#include "Devices/EEPROM_TC.h"
#include "Devices/Serial_TC.h"

// class variable
PID_TC *PID_TC::_instance = nullptr;

// implement singleton
PID_TC *PID_TC::instance() {
  if (!_instance) {
    _instance = new PID_TC();
  }
  return _instance;
}
// implement constructor
PID_TC::PID_TC() {
  double Kp, Ki, Kd;
  EEPROM_TC *eeprom = EEPROM_TC::instance();
  Kp = eeprom->getKP();
  Ki = eeprom->getKI();
  Kd = eeprom->getKD();
  if (isnan(Kp)) {
    Kp = 100000;
  }
  if (isnan(Ki)) {
    Ki = 0;
  }
  if (isnan(Kd)) {
    Kd = 0;
  }
  pPID = new PID(&input, &output, &set_point, Kp, Ki, Kd,
                 REVERSE);  // Starting the PID, Specify the links and initial
                           // tuning parameters
  pPID->SetMode(AUTOMATIC);
  pPID->SetSampleTime(1000);
  pPID->SetOutputLimits(0, WINDOW_SIZE);
}

// instance functions
double PID_TC::computeOutput(double target, double current) {
  set_point = target;
  input = current;
  pPID->Compute();
  return output;
}

void PID_TC::logToSerial() {
  Serial_TC *serial = Serial_TC::instance();
  serial->print(F("Kp:"), false);
  serial->print(pPID->GetKp());
  serial->print(F(" Ki:"), false);
  serial->print(pPID->GetKi());
  serial->print(F(" Kd:"), false);
  serial->print(pPID->GetKd());
  serial->println();
  serial->print(F("PID output (s): "), false);
  serial->print(output / 1000, 1);
  serial->println();
}

void PID_TC::setTunings(double Kp, double Ki, double Kd) {
  pPID->SetTunings(Kp, Ki, Kd);
}
