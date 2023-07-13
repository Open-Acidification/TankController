#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/DateTime_TC.h"
#include "Devices/JSONBuilder.h"
#include "Devices/PHControl.h"
#include "Devices/PHProbe.h"
#include "Devices/PID_TC.h"
#include "Devices/TempProbe_TC.h"
#include "Devices/TemperatureControl.h"
#include "TankController.h"
/**
 * Test correctness of JSON output from JSONBuilder
 */

unittest(currentData) {
  // Fake DateTime
  DateTime_TC feb(2022, 2, 22, 20, 50, 00);
  feb.setAsCurrent();
  PHProbe::instance()->setPh(8.125);                            // actual
  PHControl::instance()->setBaseTargetPh(8.25);                 // target
  TempProbe_TC::instance()->setTemperature(99.99, true);        // actual
  TemperatureControl::instance()->setTargetTemperature(98.88);  // target
  PID_TC::instance()->setTunings(100001.1, 100002.2, 100003.3);
  TankController::instance()->loop(false);                      // recognize and apply the targets
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
      "\"PHSlope\":\"\","
      "\"Kp\":100001.1,"
      "\"Ki\":100002.2,"
      "\"Kd\":100003.3,"
      "\"PID\":\"ON\","
      "\"TankID\":0,"
      "\"Uptime\":\"0d 0h 0m 1s\","
      "\"Version\":\"23.7.0 \""
      "}";
  assertEqual(expected, text);
}

unittest_main()
