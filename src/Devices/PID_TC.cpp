#include "Devices/PID_TC.h"

#include "Devices/EEPROM_TC.h"
#include "Devices/Serial_TC.h"
#include "TC_util.h"
#include "TankControllerLib.h"

// class variable
PID_TC *PID_TC::_instance = nullptr;

// implement singleton
PID_TC *PID_TC::instance() {
  if (!_instance) {
    _instance = new PID_TC();
  }
  return _instance;
}

void PID_TC::reset() {
  if (_instance) {
    delete _instance;
    _instance = nullptr;
  }
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
    eeprom->setKP(Kp);
  }
  if (isnan(Ki)) {
    Ki = 0;
    eeprom->setKI(Ki);
  }
  if (isnan(Kd)) {
    Kd = 0;
    eeprom->setKD(Kd);
  }
  pPID = new PID(&input, &output, &set_point, Kp, Ki, Kd,
                 REVERSE);  // Starting the PID, Specify the links and initial
                            // tuning parameters
  pPID->SetMode(AUTOMATIC);
  pPID->SetSampleTime(1000);
  pPID->SetOutputLimits(0, WINDOW_SIZE);
}

// implement destructor
PID_TC::~PID_TC() {
  if (pPID) {
    delete pPID;
    pPID = nullptr;
  }
}

// instance functions
double PID_TC::computeOutput(double target, double current) {
  if (TankControllerLib::instance()->isInCalibration()) {
    // current value will likely be wrong during calibration, so don't make any changes
    return output;
  }
  set_point = target;
  input = current;
  pPID->Compute();
  return output;
}

void PID_TC::logToSerial() {
  serial(F("%s %6.1f %s %6.1f %s %6.1f\r\n%s%4.1f"), F("Kp:"), pPID->GetKp(), F("Ki:"), pPID->GetKi(), F("Kd:"),
         pPID->GetKd(), F("PID output (s):"), output / 1000);
}

void PID_TC::setKd(double Kd) {
  pPID->SetTunings(getKp(), getKi(), Kd);
  EEPROM_TC::instance()->setKD(Kd);
}

void PID_TC::setKi(double Ki) {
  pPID->SetTunings(getKp(), Ki, getKd());
  EEPROM_TC::instance()->setKI(Ki);
}

void PID_TC::setKp(double Kp) {
  pPID->SetTunings(Kp, getKi(), getKd());
  EEPROM_TC::instance()->setKP(Kp);
}

void PID_TC::setTunings(double Kp, double Ki, double Kd) {
  pPID->SetTunings(Kp, Ki, Kd);
}
