#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Ethernet_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "SeeDeviceAddress.h"
#include "TankController.h"

unittest(testOutput) {
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  assertEqual("MainMenu", tc->stateName());
  SeeDeviceAddress* test = new SeeDeviceAddress(tc);
  tc->setNextState(test, true);
  assertEqual("SeeDeviceAddress", tc->stateName());

  // Test the output
  tc->loop(false);
  assertEqual("192.168.1.10    ", display->getLines().at(0));
  char buffer[8];
  strscpy(buffer, display->getLines().at(1).c_str(), sizeof(buffer));
  assertEqual("90A2:DA", buffer);

  // Reset MAC address
  GODMODE()->resetClock();
  delay(1);
  Keypad_TC::instance()->_getPuppet()->push_back('C');
  tc->loop(false);
  assertEqual("90A2:DAFC:F7F2  ", display->getLines().at(1));
  delay(1);
  Keypad_TC::instance()->_getPuppet()->push_back('C');
  tc->loop(false);
  assertEqual("90A2:DA00:FBF6  ", display->getLines().at(1));

  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
