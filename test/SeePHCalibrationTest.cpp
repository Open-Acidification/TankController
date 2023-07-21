#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "PHProbe.h"
#include "SeePHCalibration.h"
#include "TankController.h"

unittest(testOutput) {
  // Set up
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PHProbe* pPHProbe = PHProbe::instance();

  assertEqual("MainMenu", tc->stateName());
  SeePHCalibration* test = new SeePHCalibration(tc);
  tc->setNextState(test, true);
  assertEqual("SeePHCalibration", tc->stateName());

  // Test the output
  assertEqual("PH Calibration: ", display->getLines().at(0));
  tc->loop(false);
  assertEqual("Requesting...   ", display->getLines().at(1));
  pPHProbe->setCalibration(2);
  assertEqual("2 pt calibrated ", display->getLines().at(1));
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
}

unittest(testTimeout) {
  // Set up
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PHProbe* pPHProbe = PHProbe::instance();

  assertEqual("MainMenu", tc->stateName());
  SeePHCalibration* test = new SeePHCalibration(tc);
  tc->setNextState(test, true);
  assertEqual("SeePHCalibration", tc->stateName());

  pPHProbe->setCalibration(2);
  delay(55000);
  tc->loop(false);
  assertEqual("SeePHCalibration", tc->stateName());
  delay(5000);
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
