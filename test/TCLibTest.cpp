#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "EEPROM_TC.h"
#include "PHControl.h"
#include "PHProbe.h"
#include "PID_TC.h"
#include "SD_TC.h"
#include "TC_util.h"
#include "TankController.h"
#include "ThermalControl.h"
#include "ThermalProbe_TC.h"

const uint16_t TEMP_PIN = 47;
const uint16_t PH_PIN = 49;

GodmodeState *state = GODMODE();
TankController *tc = TankController::instance();
ThermalProbe_TC *thermalProbe = ThermalProbe_TC::instance();
ThermalControl *thermalControl = ThermalControl::instance();
PHProbe *pHProbe = PHProbe::instance();
PHControl *pPHControl = PHControl::instance();
SD_TC *sd = SD_TC::instance();

unittest_setup() {
  // reset time so offset is consistent
  state->resetClock();

  EEPROM_TC::instance()->setTankID(42);

  // set temperature
  thermalProbe->setTemperature(16.75, true);

  // set target temperature
  thermalControl->setThermalTarget(16.25);

  // set pH
  PHProbe::instance()->setPh(7.125);

  // set target pH
  pPHControl->enablePID(false);  // Stay on continually if needed
  pPHControl->setBaseTargetPh(7.325);

  // set Kp, Ki, and Kd
  PID_TC::instance()->setTunings(123456.7, 12345.6, 1234.5);

  // clear SD card
  sd->format();
}

unittest_teardown() {
  sd->format();
}

unittest(basicOperation) {
  // verify startup state, including that solonoids are off
  delay(1000);
  float avgTemp = static_cast<int16_t>((thermalProbe->getRunningAverage() * 100.0 + 0.5)) / 100.0;
  assertEqual(16.75, avgTemp);
  assertEqual(7.125, pHProbe->getPh());
  tc->loop(false);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_PIN]);  // solenoid off
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_PIN]);    // solenoid off

  // change targets
  thermalControl->setThermalTarget(17.25);
  pPHControl->setBaseTargetPh(6.875);

  // verify that solonoids are on
  delay(1000);
  tc->loop(false);
  delay(1000);
  tc->loop(false);
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[TEMP_PIN]);  // solenoid on
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_PIN]);    // solenoid on

  // reset targets
  thermalControl->setThermalTarget(16.25);
  pPHControl->setBaseTargetPh(7.375);

  // verify that solonoids are off
  delay(1000);
  tc->loop(false);
  delay(1000);
  tc->loop(false);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_PIN]);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_PIN]);
}

unittest(storeDataToSD) {
  // set date/time (so we can confirm data)
  DateTime_TC dt(2021, 4, 27, 14, 24, 40);
  dt.setAsCurrent();
  delay(10000);
  for (size_t i = 0; i < 4; ++i) {
    delay(500);
    tc->loop(false);
  }
  /*
    time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd
    04/27/2021 14:24:50,  42, 16.75, 16.25, 7.125, 6.825,  110, 123456.7,  12345.6,   1234.5
    04/27/2021 14:24:51,  42, 16.75, 16.25, 7.125, 6.825,  111, 123456.7,  12345.6,   1234.5
  */
  assert(sd->exists("20210427.csv"));
  File file = sd->open("20210427.csv");
  char data[4096];
  file.read(data, file.size());
  data[file.size()] = '\0';
  COUT(data);
  String contents(data), line;
  int16_t i = contents.indexOf('\n');
  line = contents.substring(0, i);
  assertEqual("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd", line.c_str());
  contents = contents.substring(i + 1);
  i = contents.indexOf('\n');
  line = contents.substring(0, i);
  String expected("04/27/2021 14:24:50,  42, 16.75, 16.25, 7.125, 7.325,   10, 123456.7,  12345.6,   1234.5");
  COUT("expectedSize = " << expected.length() << "; actualSize = " << line.length());
  assertEqual(expected, line);
  contents = contents.substring(i + 1);
  i = contents.indexOf('\n');
  line = contents.substring(0, i);
  expected = String("04/27/2021 14:24:51,  42, 16.75, 16.25, 7.125, 7.325,   11, 123456.7,  12345.6,   1234.5");
  COUT("expectedSize = " << expected.length() << "; actualSize = " << line.length());
  assertEqual(expected, line);
  COUT(data);
  file.close();
}

unittest_main()
