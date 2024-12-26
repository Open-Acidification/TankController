#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "EEPROM_TC.h"
#include "JSONBuilder.h"
#include "PHControl.h"
#include "PHProbe.h"
#include "PID_TC.h"
#include "TankController.h"
#include "ThermalControl.h"
#include "ThermalProbe_TC.h"
#include "Version.h"

/**
 * Test correctness of JSON output from JSONBuilder
 */

unittest(currentData) {
  assertTrue(1 > 0);
  // Fake DateTime
  DateTime_TC feb(2022, 2, 22, 20, 50, 00);
  feb.setAsCurrent();
  PHProbe::instance()->setPh(8.125);             // actual
  PHProbe::instance()->setPhSlope();             // actual
  PHControl::instance()->setBaseTargetPh(8.25);  // target
  PHControl::instance()->enablePID(1);
  assertTrue(2 > 0);
  ThermalProbe_TC::instance()->setTemperature(99.99, true);  // actual
  ThermalControl::instance()->setSineAmplitudeAndHours(0, 0);
  ThermalControl::instance()->setRampDurationHours(0);
  ThermalControl::instance()->setThermalTarget(98.88);  // target
  EEPROM_TC::instance()->setHeat(0);
  PID_TC::instance()->setTunings(100001.1, 100002.2, 100003.3);
  assertTrue(3 > 0);
  TankController::instance()->loop(false);  // recognize and apply the targets
  assertTrue(4 > 0);
  JSONBuilder builder;
  int size = builder.buildCurrentValues();
  assertTrue(size > 200);
  assertTrue(builder.bytesFull() == size);
  char* text = builder.bufferPtr();
  const char expected[] =
      "{"
      "\"pH\":8.125,"
      "\"Temperature\":99.99,"
      "\"pH_FunctionType\":\"FLAT\","
      "\"Target_pH\":8.25,"
      "\"pH_RampHours\":0.0,"
      "\"pH_SinePeriodHours\":0.0,"
      "\"pH_SineAmplitude\":0.0,"
      "\"Therm_FunctionType\":\"FLAT\","
      "\"TargetTemperature\":98.88,"
      "\"Therm_RampHours\":0.0,"
      "\"Therm_SinePeriodHours\":0.0,"
      "\"Therm_SineAmplitude\":0.0,"
      "\"IPAddress\":\"192.168.1.10\","
      "\"MAC\":\"90:A2:DA:80:7B:76\","
      "\"FreeMemory\":\"1024 bytes\","
      "\"GoogleSheetInterval\":65535,"
      "\"LogFile\":\"20220222.csv\","
      "\"PHSlope\":\"99.7,100.3,-0.89\","
      "\"Kp\":100001.1,"
      "\"Ki\":100002.2,"
      "\"Kd\":100003.3,"
      "\"PID\":\"ON\","
      "\"TankID\":0,"
      "\"Uptime\":\"0d 0h 0m 1s\","
      "\"HeatOrChill\":\"CHILL\","
      "\"Version\":\"" VERSION
      "\""
      ","
      "\"EditableFields\":["
      "\"Target_pH\","
      "\"TargetTemperature\","
      "\"GoogleSheetInterval\","
      "\"Kp\","
      "\"Ki\","
      "\"Kd\","
      "\"pH_RampHours\","
      "\"Therm_RampHours\","
      "\"TankID\","
      "\"HeatOrChill\","
      "\"PID\""
      "]"
      "}";
  assertEqual(expected, text);
}

unittest_main()
