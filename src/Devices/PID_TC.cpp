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
  float Kp, Ki, Kd;
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
float PID_TC::computeOutput(float target, float current) {
  if (TankControllerLib::instance()->isInCalibration()) {
    // current value will likely be wrong during calibration, so don't make any changes
    return static_cast<float>(output);
  }
  set_point = static_cast<float>(target);
  input = static_cast<double>(current);
  pPID->Compute();
  return static_cast<float>(static_cast<float>(output));
}

void PID_TC::logToSerial() {
  serial(F("Kp: %6.1f Ki: %6.1f Kd: %6.1f\r\nPID output (s):%4.1f"), pPID->GetKp(), pPID->GetKi(), pPID->GetKd(),
         static_cast<float>(output) / 1000);
}

void PID_TC::setKd(float Kd) {
  pPID->SetTunings(getKp(), getKi(), Kd);
  EEPROM_TC::instance()->setKD(Kd);
}

void PID_TC::setKi(float Ki) {
  pPID->SetTunings(getKp(), Ki, getKd());
  EEPROM_TC::instance()->setKI(Ki);
}

void PID_TC::setKp(float Kp) {
  pPID->SetTunings(Kp, getKi(), getKd());
  EEPROM_TC::instance()->setKP(Kp);
}

void PID_TC::setTunings(float Kp, float Ki, float Kd) {
  pPID->SetTunings(Kp, Ki, Kd);
}
