#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/Keypad_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "TankController.h"
#include "UIState/BadPHCalibration.h"
#include "UIState/MainMenu.h"

unittest(Display) {
  // Set up
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PHProbe* pHProbe = PHProbe::instance();
  pHProbe->setPhSlope();

  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(new BadPHCalibration(tc), true);
  assertEqual("BadPHCalibration", tc->stateName());
  assertTrue(tc->isInCalibration());

  // Test the display
  tc->loop(false);
  assertEqual("BAD CALIBRATION?", display->getLines().at(0));
  assertEqual("99.7,100.3,-0.89", display->getLines().at(1));
  delay(750);
  tc->loop(false);
  assertEqual("                ", display->getLines().at(0));
  assertEqual("99.7,100.3,-0.89", display->getLines().at(1));
  delay(250);
  tc->loop(false);
  assertEqual("BAD CALIBRATION?", display->getLines().at(0));
  assertEqual("99.7,100.3,-0.89", display->getLines().at(1));
  delay(4000);
  tc->loop(false);
  assertEqual("A: Accept/ignore", display->getLines().at(0));
  assertEqual("C: Clear calibra", display->getLines().at(1));
  delay(3000);
  tc->loop(false);
  assertEqual("BAD CALIBRATION?", display->getLines().at(0));
  assertEqual("99.7,100.3,-0.89", display->getLines().at(1));
}

unittest(Accept) {
  // Set up
  GODMODE()->reset();
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PHProbe* pHProbe = PHProbe::instance();
  pHProbe->setPhSlope();

  tc->setNextState(new BadPHCalibration(tc), true);
  assertEqual("BadPHCalibration", tc->stateName());
  assertTrue(tc->isInCalibration());
  assertEqual("", GODMODE()->serialPort[1].dataOut);

  // Type 'D'
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop(false);
  assertEqual("BadPHCalibration", tc->stateName());

  // Type 'A'
  Keypad_TC::instance()->_getPuppet()->push_back('A');
  tc->loop(false);
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
  assertEqual("", GODMODE()->serialPort[1].dataOut);
  assertTrue(tc->getIgnoreBadPHSlope());
}

unittest(Clear) {
  // Set up
  GODMODE()->reset();
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PHProbe* pHProbe = PHProbe::instance();
  pHProbe->setPhSlope();

  tc->setNextState(new BadPHCalibration(tc), true);
  assertEqual("BadPHCalibration", tc->stateName());
  assertTrue(tc->isInCalibration());

  // Type 'B'
  Keypad_TC::instance()->_getPuppet()->push_back('B');
  tc->loop(false);
  assertEqual("BadPHCalibration", tc->stateName());

  // Type 'C'
  assertEqual("", GODMODE()->serialPort[1].dataOut);
  Keypad_TC::instance()->_getPuppet()->push_back('C');
  tc->loop(false);
  assertEqual("SeePHCalibration", tc->stateName());
  assertEqual("Cal,clear\rCAL,?\rSLOPE,?\r", GODMODE()->serialPort[1].dataOut);
  assertFalse(tc->getIgnoreBadPHSlope());
}

unittest(CatchBadCalibration) {
  GODMODE()->reset();
  TankController* tc = TankController::instance();
  PHProbe* pHProbe = PHProbe::instance();
  pHProbe->setPhSlope();
  assertFalse(tc->getIgnoreBadPHSlope());
  assertFalse(pHProbe->slopeIsBad());

  tc->setNextState(new MainMenu(tc));
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
  assertFalse(tc->isInCalibration());
  pHProbe->setPhSlope("?SLOPE,99.7,0");  // 0% base slope is outside range
  assertTrue(pHProbe->slopeIsBad());
  assertEqual("MainMenu", tc->stateName());
  tc->loop(false);  // catch flag and queue next state
  tc->loop(false);  // make new state active
  assertEqual("BadPHCalibration", tc->stateName());
}

unittest(IgnoreBadCalibration) {
  GODMODE()->reset();
  TankController* tc = TankController::instance();
  PHProbe* pHProbe = PHProbe::instance();
  pHProbe->setPhSlope();
  assertFalse(tc->getIgnoreBadPHSlope());
  assertFalse(pHProbe->slopeIsBad());
  tc->setIgnoreBadPHSlope(true);
  assertTrue(tc->getIgnoreBadPHSlope());

  tc->setNextState(new MainMenu(tc));
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
  assertFalse(tc->isInCalibration());
  pHProbe->setPhSlope("?SLOPE,99.7,0");  // 0% base slope is outside range
  assertTrue(pHProbe->slopeIsBad());
  assertEqual("MainMenu", tc->stateName());
  tc->loop(false);  // ignore bad calibration flag
  tc->loop(false);  // continue to ignore flag
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
