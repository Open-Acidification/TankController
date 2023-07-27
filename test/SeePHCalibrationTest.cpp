#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/Keypad_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "Devices/Serial_TC.h"
#include "TankController.h"
#include "UIState/SeePHCalibration.h"

unittest(testOutput) {
  // Set up
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PHProbe* pPHProbe = PHProbe::instance();

  assertEqual("MainMenu", tc->stateName());
  SeePHCalibration* state = new SeePHCalibration(tc);
  tc->setNextState(state, true);
  assertEqual("SeePHCalibration", tc->stateName());
  assertFalse(tc->isInCalibration());

  // Test the output
  tc->loop(false);
  assertEqual("Requesting...   ", display->getLines().at(0));
  assertEqual("Requesting...   ", display->getLines().at(1));
  pPHProbe->setCalibration(2);
  pPHProbe->setPhSlope();
  assertEqual("2 pt calibrated ", display->getLines().at(0));
  assertEqual("99.7,100.3,-0.89", display->getLines().at(1));
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
  SeePHCalibration* state = new SeePHCalibration(tc, true);
  tc->setNextState(state, true);
  assertEqual("SeePHCalibration", tc->stateName());

  pPHProbe->setCalibration(2);
  delay(55000);
  tc->loop(false);
  assertTrue(tc->isInCalibration());
  assertEqual("SeePHCalibration", tc->stateName());
  delay(5000);
  tc->loop(false);  // Set next state
  tc->loop(false);  // Loop again to switch states
  assertEqual("MainMenu", tc->stateName());
  assertFalse(tc->isInCalibration());
}

unittest(checkPhSlope) {
  // Set up
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PHProbe* pPHProbe = PHProbe::instance();

  assertEqual("MainMenu", tc->stateName());
  SeePHCalibration* state = new SeePHCalibration(tc, true);
  tc->setNextState(state, true);
  assertEqual("SeePHCalibration", tc->stateName());

  // slopeResponse should say "Requesting..."
  state->checkPhSlope();
  assertEqual("SeePHCalibration::checkPhSlope() failed to parse slope from PHProbe::instance()",
              Serial_TC::instance()->buffer);
  // set slopes to good values
  pPHProbe->setCalibration(3);
  pPHProbe->setPhSlope();
  state->checkPhSlope();
  assertEqual("pH slopes are within 5\% of ideal", Serial_TC::instance()->buffer);
  // set slope to bad value
  pPHProbe->setPhSlope("?SLOPE,94.3,100.3,-0.89\r");
  state->checkPhSlope();
  assertEqual("BAD CALIBRATION? pH slopes are more than 5\% from ideal", Serial_TC::instance()->buffer);
  // set slope to bad value
  pPHProbe->setPhSlope("?SLOPE,105.7,100.3,-0.89\r");
  state->checkPhSlope();
  assertEqual("BAD CALIBRATION? pH slopes are more than 5\% from ideal", Serial_TC::instance()->buffer);
  // set slope to bad value
  pPHProbe->setPhSlope("?SLOPE,99.7,10,-0.89\r");
  state->checkPhSlope();
  assertEqual("BAD CALIBRATION? pH slopes are more than 5\% from ideal", Serial_TC::instance()->buffer);
  // set slope to bad value
  pPHProbe->setPhSlope("?SLOPE,99.7,105.9,-0.89\r");
  state->checkPhSlope();
  assertEqual("BAD CALIBRATION? pH slopes are more than 5\% from ideal", Serial_TC::instance()->buffer);
  // set slopes to good values
  pPHProbe->setPhSlope();
  state->checkPhSlope();
  assertEqual("pH slopes are within 5\% of ideal", Serial_TC::instance()->buffer);
  // tc->loop();
  // assertEqual("MainMenu", tc->stateName());
}

unittest_main()
