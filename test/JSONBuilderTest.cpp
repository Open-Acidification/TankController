#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/JSONBuilder.h"
#include "TankController.h"
/**
 * Test correctness of JSON output from JSONBuilder
 */

unittest(current) {
  TankController* tc = TankController::instance();
  JSONBuilder builder;
  int size = builder.buildCurrentValues();
  assertTrue(size > 200);
  assertTrue(builder.bytesFull() == size);
  char* text = builder.bufferPtr();
  // const char response[]
  const char expected[] =
      "{\"IPAddress\":\"192.168.1.10\","
      "\"MAC\":\"90:A2:DA:80:7B:76\","
      "\"FreeMemory\":\"64 bytes\","
      "\"GoogleSheetInterval\":65535,"
      "\"LogFile\":\"\","
      "\"PHSlope\":\"\","
      "\"Kp\":100000.0,"
      "\"Ki\":0.0,"
      "\"Kd\":0.0,"
      "\"PID\":\"ON\","
      "\"TankID\":0,"
      "\"Uptime\":\"0d 0h 0m 1s\","
      "\"Version\":\"21.09.1\"}";
  assertEqual(expected, text);
}

unittest_main()
