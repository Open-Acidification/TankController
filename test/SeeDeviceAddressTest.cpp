#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "SeeDeviceAddress.h"
#include "TankControllerLib.h"

unittest(testOutput) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());
  SeeDeviceAddress* test = new SeeDeviceAddress(tc);
  tc->setNextState(test, true);
  assertEqual("SeeDeviceAddress", tc->stateName());

  // Test the output
  tc->loop();
  assertEqual("192.168.001.010 ", display->getLines().at(0));
  assertEqual("90A2:DA00:0000  ", display->getLines().at(1));
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
