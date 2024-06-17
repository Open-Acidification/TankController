#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
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
  // Fake DateTime
  DateTime_TC feb(2022, 2, 22, 20, 50, 00);
  feb.setAsCurrent();
  PHProbe::instance()->setPh(8.125);                         // actual
  PHProbe::instance()->setPhSlope();                         // actual
  PHControl::instance()->setBaseTargetPh(8.25);              // target
  ThermalProbe_TC::instance()->setTemperature(99.99, true);  // actual
  ThermalControl::instance()->setThermalTarget(98.88);       // target
  PID_TC::instance()->setTunings(100001.1, 100002.2, 100003.3);
  TankController::instance()->loop(false);  // recognize and apply the targets
  JSONBuilder builder;
  int size = builder.buildCurrentValues();
  assertTrue(size > 200);
  assertTrue(builder.bytesFull() == size);
  char* text = builder.bufferPtr();
  const char expected[] =
      "{"
      "\"pH\":8.125,"
      "\"Target_pH\":8.25,"
      "\"Temperature\":99.99,"
      "\"TargetTemperature\":98.88,"
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
      "\"Version\":\"" VERSION
      "\","
      "\"EditableFields\":[\"Target_pH\",\"TargetTemperature\",\"GoogleSheetInterval\",\"Kp\",\"Ki\",\"Kd\",\"TankID\"]"
      "}";
  assertEqual(expected, text);
}

unittest_main()
