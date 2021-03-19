#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/Keypad_TC.h"
#include "TankControllerLib.h"

unittest(SetPHSetPoint) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  lines = lc->getLines();
  assertEqual(" ONTHANK LAB", lc->getLines().at(0).substr(4));
  keypad->push_back('A');
  tc->loop();  // recognize and apply the key entry
  assertEqual("Set pH Set Point", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(SetTempSetPoint) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('B');
  tc->loop();  // recognize and apply the key entry
  assertEqual("Set Temperature ", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(PHCalibration) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('C');
  tc->loop();  // recognize and apply the key entry
  assertEqual("pH-Calibration  ", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(CalibrationManagement) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('D');
  tc->loop();  // recognize and apply the key entry
  assertEqual("Cal Management  ", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(SetTankID) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('#');
  tc->loop();  // recognize and apply the key entry
  assertEqual("Set Tank ID#    ", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(ChangeGoogleSheetInterval) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('*');
  tc->loop();  // recognize and apply the key entry
  assertEqual("G Sheet Interval", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(SeeDeviceUptime) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('0');
  tc->loop();  // recognize and apply the key entry
  assertEqual("Device time     ", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(SeeDeviceAddress) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('1');
  tc->loop();  // recognize and apply the key entry
  assertEqual("Device address  ", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(ResetLCDScreen) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('2');
  tc->loop();  // recognize and apply the key entry
  assertEqual("Clearing Screen ", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(SeeTankID) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('3');
  tc->loop();  // recognize and apply the key entry
  assertEqual("Tank ID=        ", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(SeePIDConstants) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('4');
  tc->loop();  // recognize and apply the key entry
  assertEqual("PID Constants   ", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(PIDTuningMenu) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('5');
  tc->loop();  // recognize and apply the key entry
  assertEqual("PID TUNING      ", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(TemperatureCalibration) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('6');
  tc->loop();  // recognize and apply the key entry
  assertEqual("Temp Calibration", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(SetTime) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('7');
  tc->loop();  // recognize and apply the key entry
  assertEqual("Set Year (YYYY):", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(EnablePID) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('8');
  tc->loop();  // recognize and apply the key entry
  assertEqual("Enable PID?     ", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest(SetChillOrHeat) {
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
  Keypad* keypad = Keypad_TC::instance()->_getPuppet();
  std::vector<String> lines;
  tc->loop();
  keypad->push_back('9');
  tc->loop();  // recognize and apply the key entry
  assertEqual("Chill or Heat?  ", lc->getLines().at(0));
  keypad->push_back('D');  // Don't finish (cancel)
  tc->loop();              // recognize and apply the key entry
  lines = lc->getLines();
  assertEqual("Main Menu       ", lc->getLines().at(0));
}

unittest_main()
