#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/DateTime_TC.h"
#include "Devices/JSONBuilder.h"
#include "TankController.h"
/**
 * Test correctness of JSON output from JSONBuilder
 */

unittest(current) {
  // Fake DateTime
  DateTime_TC feb(2022, 2, 22, 20, 50, 00);
  feb.setAsCurrent();
  JSONBuilder builder;
  int size = builder.buildCurrentValues();
  assertTrue(size > 200);
  assertTrue(builder.bytesFull() == size);
  char* text = builder.bufferPtr();
  const char expected[] =
      "{\"IPAddress\":\"192.168.1.10\","
      "\"MAC\":\"90:A2:DA:FB:F6:F1\","
      "\"FreeMemory\":\"1024 bytes\","
      "\"GoogleSheetInterval\":65535,"
      "\"LogFile\":\"20220222.csv\","
      "\"PHSlope\":\"\","
      "\"Kp\":100000.0,"
      "\"Ki\":0.0,"
      "\"Kd\":0.0,"
      "\"PID\":\"ON\","
      "\"TankID\":0,"
      "\"Uptime\":\"0d 0h 0m 1s\","
      "\"Version\":\"22.03.1\"}";
  assertEqual(expected, text);
}

unittest_main()
