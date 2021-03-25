#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
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

unittest(MainMenu) {
  TempProbe_TC::instance()->setTemperature(12.30);
  assertEqual(" ONTHANK LAB", lc->getLines().at(0).substr(4));
  enterKey('A');
  enterKey('D');
  assertEqual("Main Menu       ", lc->getLines().at(0));
  assertEqual("Temp=12.30      ", lc->getLines().at(1));
}

unittest(SetPHSetPoint) {
  enterKey('A');
  assertEqual("Set pH Set Point", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(SetTempSetPoint) {
  enterKey('B');
  assertEqual("Set Temperature ", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(PHCalibration) {
  enterKey('C');
  assertEqual("pH-Calibration  ", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(CalibrationManagement) {
  enterKey('D');
  assertEqual("Cal Management  ", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(SetTankID) {
  enterKey('#');
  assertEqual("Set Tank ID#    ", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(SetGoogleSheetInterval) {
  enterKey('*');
  assertEqual("G Sheet Minutes ", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(SeeDeviceUptime) {
  enterKey('0');
  assertEqual(DateTime_TC::now().as16CharacterString(), lc->getLines().at(0).c_str());
  delay(6000);
  tc->loop();  // this will set MainMenu as the next state
  tc->loop();  // this will start MainMenu
  assertEqual("MainMenu", tc->stateName());
}

unittest(SeeDeviceAddress) {
  enterKey('1');
  assertEqual("Device address  ", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(ResetLCDScreen) {
  enterKey('2');
  assertEqual("Clearing Screen ", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(SeeTankID) {
  enterKey('3');
  assertEqual("Tank ID=        ", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(SeePIDConstants) {
  enterKey('4');
  assertEqual("PID Constants   ", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(PIDTuningMenu) {
  enterKey('5');
  assertEqual("PID TUNING      ", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(TemperatureCalibration) {
  enterKey('6');
  assertEqual("Temp Calibration", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(SetTime) {
  enterKey('7');
  assertEqual("Set Year (YYYY):", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(EnablePID) {
  enterKey('8');
  assertEqual("Enable PID?     ", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(SetChillOrHeat) {
  enterKey('9');
  assertEqual("1:Chill; 9:Heat ", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
