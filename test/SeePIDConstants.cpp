#include "SeePIDConstants.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "LiquidCrystal_TC.h"
#include "PHProbe.h"
#include "PID_TC.h"
#include "TankControllerLib.h"
#include "UIState/UIState.h"

unittest(TestVerticalScroll) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PID* pPID = PID_TC::instance()->getPID();
  SeePIDConstants* test = new SeePIDConstants(tc);
  char lines[2][17];

  // Set up
  double kp = pPID->GetKp();
  double ki = pPID->GetKi();
  double kd = pPID->GetKd();
  pPID->SetTunings(10000.0, 10000.0, 10000.0);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeePIDConstants nextState: Wait
  tc->loop();
  assertEqual("SeePIDConstants", tc->stateName());

  // during the delay we cycle through kp,ki,kd, and ph slope
  assertEqual("kp:10000.0      ", display->getLines().at(0));
  assertEqual("ki:10000.0      ", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("ki:10000.0      ", display->getLines().at(0));
  assertEqual("kd:10000.0      ", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("kd:10000.0      ", display->getLines().at(0));
  assertEqual("Slope:0.0       ", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("Slope:0.0       ", display->getLines().at(0));
  assertEqual("kp:10000.0      ", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("kp:10000.0      ", display->getLines().at(0));
  assertEqual("ki:10000.0      ", display->getLines().at(1));

  delay(1000);
  tc->loop();  // SeePIDConstants nextState: MainMenu
  tc->loop();  // SeePIDConstants -> MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());

  // Clean up
  pPID->SetTunings(kp, ki, kd);
}

unittest_main()
