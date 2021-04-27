#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/PHProbe.h"
#include "PHControl.h"
#include "TankControllerLib.h"
#include "TempProbe_TC.h"
#include "TemperatureControl.h"

const int TEMP_PIN = 47;
const int PH_PIN = 49;

GodmodeState *state = GODMODE();
TankControllerLib *pTC = TankControllerLib::instance();
TempProbe_TC *tempProbe = TempProbe_TC::instance();
TemperatureControl *tempControl = TemperatureControl::instance();
PHProbe *pPHProbe = PHProbe::instance();
PHControl *pPHControl = PHControl::instance();

unittest_setup() {
  pTC->setCalibrationMode(false);
}

unittest_teardown() {
  pTC->setCalibrationMode(false);
}

unittest(test) {
  // set temperature
  tempProbe->setTemperature(20.0);
  tempProbe->setCorrection(0.0);
  for (int i = 0; i < 100; ++i) {
    tempProbe->getRunningAverage();
  }
  assertEqual(20, (int)tempProbe->getRunningAverage());

  // set target temperature
  tempControl->setTargetTemperature(20.0);

  // set pH
  state->serialPort[1].dataIn = "7.5\r";  // the queue of data waiting to be read
  pTC->serialEvent1();                    // fake interrupt
  assertEqual(7.5, pPHProbe->getPh());

  // set target pH
  pPHControl->setUsePID(false);
  pPHControl->setTargetPh(7.5);

  // verify that solonoids are off
  pTC->loop();
  assertEqual(HIGH, state->digitalPin[TEMP_PIN]);
  assertEqual(HIGH, state->digitalPin[PH_PIN]);

  // change targets
  tempControl->setTargetTemperature(21.0);
  pPHControl->setTargetPh(7.4);
  // verify that solonoids are on
  pTC->loop();
  delay(10);
  pTC->loop();
  assertEqual(LOW, state->digitalPin[TEMP_PIN]);
  assertEqual(LOW, state->digitalPin[PH_PIN]);

  // reset targets
  tempControl->setTargetTemperature(19.0);
  pPHControl->setTargetPh(7.6);
  // verify that solonoids are off
  pTC->loop();
  delay(10);
  pTC->loop();
  assertEqual(HIGH, state->digitalPin[TEMP_PIN]);
  assertEqual(HIGH, state->digitalPin[PH_PIN]);

  pTC->setCalibrationMode(true);
  // change targets
  tempControl->setTargetTemperature(21.0);
  pPHControl->setTargetPh(7.4);
  // verify that solonoids remain off
  pTC->loop();
  delay(10);
  pTC->loop();
  assertEqual(HIGH, state->digitalPin[TEMP_PIN]);
  assertEqual(HIGH, state->digitalPin[PH_PIN]);
  pTC->setCalibrationMode(false);
}

unittest_main()
