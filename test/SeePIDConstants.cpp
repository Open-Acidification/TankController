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
  GODMODE()->serialPort[1].dataIn = "?Slope,99.7,100.3,-0.89\r";  // the queue of data waiting to be read
  tc->serialEvent1();                                             // fake interrupt

  char lines[2][17];

  // Set up
  double kp = pPID->GetKp();
  double ki = pPID->GetKi();
  double kd = pPID->GetKd();
  pPID->SetTunings(10001.0, 10002.0, 10003.0);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeePIDConstants nextState: Wait
  tc->loop();
  assertEqual("SeePIDConstants", tc->stateName());

  // during the delay we cycle through kp,ki,kd, and ph slope
  assertEqual("Kp: 10001.0     ", display->getLines().at(0));
  assertEqual("Ki: 10002.0     ", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("Ki: 10002.0     ", display->getLines().at(0));
  assertEqual("Kd: 10003.0     ", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("Kd: 10003.0     ", display->getLines().at(0));
  assertEqual("99.7,100.3,-0.89", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("99.7,100.3,-0.89", display->getLines().at(0));
  assertEqual("Kp: 10001.0     ", display->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("Kp: 10001.0     ", display->getLines().at(0));
  assertEqual("Ki: 10002.0     ", display->getLines().at(1));

  delay(1000);
  tc->loop();  // SeePIDConstants nextState: MainMenu
  tc->loop();  // SeePIDConstants -> MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());

  // Clean up
  pPID->SetTunings(kp, ki, kd);
}

unittest_main()
