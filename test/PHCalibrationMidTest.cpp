#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "PHCalibrationMid.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLib* tc = TankControllerLib::instance();
  PHCalibrationMid* test = new PHCalibrationMid(tc);
  tc->setNextState(test, true);
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New Mid = 12.345", lines[1]);
  // assertEqual("PHCalibrationMid", tc->stateName());
  // tc->loop();  // transition to Wait
  // assertEqual("Wait", tc->stateName());
  // delay(1000);
  // assertEqual("PHCalibrationLow", tc->stateName());
  // tc->loop();  // queue MainMenu to be next
  // tc->loop();  // transition to MainMenu
  // // now we should be back to the main menu
  // assertEqual("MainMenu", tc->stateName());
}

unittest_main()
