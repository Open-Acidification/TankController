#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "PHControl.h"
#include "PHProbe.h"
#include "SD_TC.h"
#include "TC_util.h"
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
SD_TC *sd = SD_TC::instance();

unittest_setup() {
  // set temperature
  tempProbe->setTemperature(20.0);
  tempProbe->setCorrection(0.0);
  for (int i = 0; i < 100; ++i) {
    delay(1000);
    tempProbe->getRunningAverage();
  }

  // set target temperature
  tempControl->setTargetTemperature(20.0);

  // set pH
  state->serialPort[1].dataIn = "7.5\r";  // the queue of data waiting to be read
  pTC->serialEvent1();                    // fake interrupt

  // set target pH
  pPHControl->enablePID(false);
  pPHControl->setTargetPh(7.5);

  // clear SD card
  SD.removeAll();
}

unittest_teardown() {
  SD.removeAll();
}

unittest(basicOperation) {
  // verify startup state, including that solonoids are off
  delay(1000);
  assertEqual(20, (int)tempProbe->getRunningAverage());
  assertEqual(7.5, pPHProbe->getPh());
  pPHControl->enablePID(false);  // Stay on continually if needed
  pTC->loop();
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_PIN]);  // solenoid off
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_PIN]);    // solenoid off

  // change targets
  tempControl->setTargetTemperature(21.0);
  pPHControl->setTargetPh(7.4);

  // verify that solonoids are on
  delay(1000);
  pTC->loop();
  delay(1000);
  pTC->loop();
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[TEMP_PIN]);  // solenoid on
  assertEqual(TURN_SOLENOID_ON, state->digitalPin[PH_PIN]);    // solenoid on

  // reset targets
  tempControl->setTargetTemperature(19.0);
  pPHControl->setTargetPh(7.6);

  // verify that solonoids are off
  delay(1000);
  pTC->loop();
  delay(1000);
  pTC->loop();
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[TEMP_PIN]);
  assertEqual(TURN_SOLENOID_OFF, state->digitalPin[PH_PIN]);
}

unittest(storeDataToSD) {
  // set date/time (so we can confirm data)
  DateTime_TC dt(2021, 4, 27, 14, 24, 50);
  dt.setAsCurrent();
  for (int i = 0; i < 4; ++i) {
    delay(500);
    pTC->loop();
  }
  /*
  time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd
  04/27/2021 14:24:50,   0, 20.021, 20.000, 7.5000, 7.5000,    0,   0.0,   0.0,   0.0
  04/27/2021 14:24:51,   0, 20.021, 20.000, 7.5000, 7.5000,    0,   0.0,   0.0,   0.0
  */
  File file = SD.open("20210427.csv");
  char data[4096];
  file.read(data, file.size());
  data[file.size()] = '\0';
  COUT(data);
  String contents(data), line;
  int i = contents.indexOf('\n');
  line = contents.substring(0, i);
  assertEqual("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd", line.c_str());
  contents = contents.substring(i + 1);
  i = contents.indexOf('\n');
  line = contents.substring(0, i);
  String expected("04/27/2021 14:24:50,   0, 20.021, 20.000, 7.5000, 7.5000,    0, 100000.0,   0.0,   0.0");
  COUT("expectedSize = " << expected.length() << "; actualSize = " << line.length());
  assertEqual(expected, line);
  contents = contents.substring(i + 1);
  i = contents.indexOf('\n');
  line = contents.substring(0, i);
  expected = String("04/27/2021 14:24:51,   0, 20.021, 20.000, 7.5000, 7.5000,    0, 100000.0,   0.0,   0.0");
  COUT("expectedSize = " << expected.length() << "; actualSize = " << line.length());
  assertEqual(expected, line);
  COUT(data);
  file.close();
}

unittest_main()
