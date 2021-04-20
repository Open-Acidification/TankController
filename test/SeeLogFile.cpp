#include "SeeLogFile.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "TankControllerLib.h"

unittest(testOutput) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  DateTime_TC now = DateTime_TC::now();
  assertEqual("MainMenu", tc->stateName());
  SeeLogFile* test = new SeeLogFile(tc);
  tc->setNextState(test, true);
  assertEqual("SeeLogFile", tc->stateName());

  // Test the output
  char reference[17];
  sprintf(reference, "%02i/%02i.txt       ", now.month(), now.day());
  tc->loop();
  assertEqual("Current Log File", display->getLines().at(0));
  assertEqual(reference, display->getLines().at(1).c_str());
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
