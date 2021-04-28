#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <PID_v1.h>

#include "Devices/DateTime_TC.h"
#include "Devices/PID_TC.h"
#include "Devices/Serial_TC.h"
#include "TC_util.h"

unittest(singleton) {
  PID_TC *singleton1 = PID_TC::instance();
  PID_TC *singleton2 = PID_TC::instance();
  assertEqual(singleton1, singleton2);
}

unittest(constructor) {
  PID_TC *singleton = PID_TC::instance();
  PID *pPID = singleton->getPID();
  assertEqual(100000.0, pPID->GetKp());
  assertEqual(0.0, pPID->GetKi());
  assertEqual(0.0, pPID->GetKd());
  assertEqual(AUTOMATIC, pPID->GetMode());
}

unittest(logToSerial) {
  GodmodeState *state = GODMODE();
  DateTime_TC::now();                 // this puts stuff on the serial port that we want to ignore
  state->serialPort[0].dataOut = "";  // so here we ignore it!
  PID_TC *singleton = PID_TC::instance();
  singleton->logToSerial();
  assertEqual("Kp: 100000.0 Ki:    0.0 Kd:    0.0\r\nPID output (s): 0.0\r\n", state->serialPort[0].dataOut);
}

unittest(compute) {
  PID_TC *pPID = PID_TC::instance();
  pPID->setTunings(2, 5, 1);
  // initialize the variables we're linked to
  double input = 50;
  double setpoint = 100;
  double output;

  // turn the PID on
  for (int i = 0; i < 1000; i++) {
    delay(200);
    output = pPID->computeOutput(setpoint, input);
    double delta = output - input;
    input = input - (delta / 25.6);
  }
  assertEqual(setpoint, round(input));
}

unittest(setTunings) {
  PID_TC *singleton = PID_TC::instance();
  PID *pPID = singleton->getPID();
  singleton->setTunings(2, 5, 1);
  assertEqual(2, pPID->GetKp());
  assertEqual(5, pPID->GetKi());
  assertEqual(1, pPID->GetKd());
  singleton->setTunings(20, 50, 10);
  assertEqual(20, pPID->GetKp());
  assertEqual(50, pPID->GetKi());
  assertEqual(10, pPID->GetKd());
}

unittest(setKp) {
  PID_TC *singleton = PID_TC::instance();
  PID *pPID = singleton->getPID();
  singleton->setKp(2);
  assertEqual(2, pPID->GetKp());
  assertEqual(0, pPID->GetKi());
  assertEqual(0, pPID->GetKd());
}

unittest(setKi) {
  PID_TC *singleton = PID_TC::instance();
  PID *pPID = singleton->getPID();
  singleton->setKi(5);
  assertEqual(5, pPID->GetKi());
  assertEqual(2, pPID->GetKp());
  assertEqual(0, pPID->GetKd());
}

unittest(setKd) {
  PID_TC *singleton = PID_TC::instance();
  PID *pPID = singleton->getPID();
  singleton->setKd(1);
  assertEqual(1, pPID->GetKd());
  assertEqual(2, pPID->GetKp());
  assertEqual(5, pPID->GetKi());
}

unittest_main()
