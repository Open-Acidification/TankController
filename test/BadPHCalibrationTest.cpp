#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/Keypad_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "TankController.h"
#include "UIState/BadPHCalibration.h"

unittest(testDisplay) {
  // Set up
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PHProbe* pPHProbe = PHProbe::instance();
  pPHProbe->setPhSlope();

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
  assertEqual("A: Accept anyway", display->getLines().at(0));
  assertEqual("C: Clear/reset  ", display->getLines().at(1));
  delay(3000);
  tc->loop(false);
  assertEqual("BAD CALIBRATION?", display->getLines().at(0));
  assertEqual("99.7,100.3,-0.89", display->getLines().at(1));
}

unittest(testAccept) {
  // Set up
  GODMODE()->reset();
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PHProbe* pPHProbe = PHProbe::instance();
  pPHProbe->setPhSlope();

  assertEqual("MainMenu", tc->stateName());
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
  assertFalse(tc->giveWarningForPHSlope());
}

unittest(testClear) {
  // Set up
  GODMODE()->reset();
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PHProbe* pPHProbe = PHProbe::instance();
  pPHProbe->setPhSlope();

  assertEqual("MainMenu", tc->stateName());
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
  assertEqual("Cal,clear\r", GODMODE()->serialPort[1].dataOut);
  assertEqual("SeePHCalibration", tc->stateName());
  assertTrue(tc->giveWarningForPHSlope());
}

unittest_main()
