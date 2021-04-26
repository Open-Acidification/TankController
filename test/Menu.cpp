#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "Devices/TemperatureControl.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "TankControllerLib.h"
#include "TempProbe_TC.h"

// globals for the singletons used in every test
TankControllerLib* tc = TankControllerLib::instance();
LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
Keypad* keypad = Keypad_TC::instance()->_getPuppet();

// reduce duplicate code and make it more explicit
void enterKey(char key) {
  keypad->push_back(key);
  tc->loop();  // recognize and apply the key entry
}

unittest_setup() {
  TempProbe_TC::instance()->setTemperature(12.25);
  TemperatureControl::enableHeater(true);
  TemperatureControl::instance()->setTargetTemperature(15.75);
  enterKey('D');
}

unittest_teardown() {
  enterKey('D');
}

unittest(MainMenu) {
  assertEqual("pH=0.000   7.125", lc->getLines().at(0));
  assertEqual("T=12.23  H 15.75", lc->getLines().at(1));
}

unittest(ChangeSettings) {
  enterKey('2');
  assertEqual("Change settings ", lc->getLines().at(0));
  assertEqual("<4   ^2  8v   6>", lc->getLines().at(1));
  enterKey('D');
  assertEqual("pH=0.000   7.125", lc->getLines().at(0));
  assertEqual("T=12.23  H 15.75", lc->getLines().at(1));
  enterKey('8');
  enterKey('8');
  assertEqual("Change settings ", lc->getLines().at(0));
  assertEqual("<4   ^2  8v   6>", lc->getLines().at(1));
  enterKey('4');
  assertEqual("pH=0.000   7.125", lc->getLines().at(0));
  assertEqual("T=12.23  H 15.75", lc->getLines().at(1));
}

unittest(ViewSettings) {
  enterKey('8');
  assertEqual("View TC settings", lc->getLines().at(0));
  assertEqual("<4   ^2  8v   6>", lc->getLines().at(1));
  enterKey('D');
  assertEqual("pH=0.000   7.125", lc->getLines().at(0));
  assertEqual("T=12.23  H 15.75", lc->getLines().at(1));
  enterKey('2');
  enterKey('2');
  assertEqual("View TC settings", lc->getLines().at(0));
  assertEqual("<4   ^2  8v   6>", lc->getLines().at(1));
  enterKey('D');
  assertEqual("pH=0.000   7.125", lc->getLines().at(0));
  assertEqual("T=12.23  H 15.75", lc->getLines().at(1));
}

unittest(SetPHSetPoint) {
  enterKey('A');
  assertEqual("Set pH Set Point", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
  enterKey('2');
  enterKey('6');
  assertEqual("Set pH target   ", lc->getLines().at(0));
  assertEqual("<4   ^2  8v   6>", lc->getLines().at(1));
  enterKey('6');
  assertEqual("Set pH Set Point", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(ViewTime) {
  enterKey('8');
  enterKey('6');
  assertEqual("View time       ", lc->getLines().at(0));
  assertEqual("<4   ^2  8v   6>", lc->getLines().at(1));
  enterKey('6');
  assertEqual(DateTime_TC::now().as16CharacterString(), lc->getLines().at(0).c_str());
  delay(6000);
  tc->loop();  // this will set MainMenu as the next state
  tc->loop();  // this will start MainMenu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
