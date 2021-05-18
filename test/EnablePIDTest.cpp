#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "EnablePID.h"
#include "TankControllerLib.h"

unittest(EnablePID) {
  TankControllerLib* tc = TankControllerLib::instance();
  EnablePID* test = new EnablePID(tc);
  tc->setNextState(test, true);
  assertTrue(PHControl::instance()->getUsePID());

  test->setValue(2.0);
  assertTrue(PHControl::instance()->getUsePID());

  test->setValue(9.0);
  assertFalse(PHControl::instance()->getUsePID());

  test->setValue(2.0);
  assertFalse(PHControl::instance()->getUsePID());

  test->setValue(1.0);
  assertTrue(PHControl::instance()->getUsePID());
}

unittest_main()
