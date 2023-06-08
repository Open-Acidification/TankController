#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "SeeLogFile.h"
#include "TankController.h"

unittest(testOutput) {
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  DateTime_TC now = DateTime_TC::now();
  assertEqual("MainMenu", tc->stateName());
  SeeLogFile* test = new SeeLogFile(tc);
  tc->setNextState(test, true);
  assertEqual("SeeLogFile", tc->stateName());

  // Test the output
  char reference[17];
  snprintf_P(reference, sizeof(reference), (PGM_P)F("%4i%02i%02i.csv       "), now.year(), now.month(), now.day());
  tc->loop(false);
  assertEqual("Current Log File", display->getLines().at(0));
  assertEqual(reference, display->getLines().at(1).c_str());
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
