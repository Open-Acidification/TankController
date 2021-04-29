#include "SeePIDConstants.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "PID_TC.h"
#include "TankControllerLib.h"
#include "UIState/UIState.h"

unittest(TestVerticalScroll) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PID_TC* pPID = PID_TC::instance();
  SeePIDConstants* test = new SeePIDConstants(tc);

  // Set up
  double kp = pPID->getKp();
  double ki = pPID->getKi();
  double kd = pPID->getKd();
  pPID->SetTunings(10001.0, 10002.0, 10003.0);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeePIDConstants nextState: Wait
  tc->loop();
  assertEqual("SeePIDConstants", tc->stateName());

  // during the delay we cycle through kp,ki, and kd
  assertEqual("Kp: 10001.0     ", display->getLines().at(0));
  assertEqual("Ki: 10002.0     ", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("Kp: 10001.0     ", display->getLines().at(0));
  assertEqual("Ki: 10002.0     ", display->getLines().at(1));
  delay(2000);
  tc->loop();
  assertEqual("Kd: 10003.0     ", display->getLines().at(0));
  assertEqual("                ", display->getLines().at(1));
  delay(3000);
  tc->loop();
  assertEqual("Kp: 10001.0     ", display->getLines().at(0));
  assertEqual("Ki: 10002.0     ", display->getLines().at(1));

  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());

  // Clean up
  pPID->setTunings(kp, ki, kd);
}

unittest_main()
