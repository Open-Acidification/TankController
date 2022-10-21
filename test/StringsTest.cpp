#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "TC_util.h"

GodmodeState* state = GODMODE();

unittest(stringCopy) {
  static const char source[11] = "stringtest";
  char dest1[9];
  char dest2[] = "012345678";
  char dest3[] = "0123456789abc";
  char dest4[] = "0123456789abc";

  assertEqual(1, strscpy(dest1, source, sizeof(dest1)));
  assertEqual("stringte", dest1);
  assertEqual('\0', dest1[8]);
  assertEqual(1, strscpy(dest2, source, sizeof(dest2)));
  assertEqual("stringtes", dest2);
  assertEqual('\0', dest2[9]);
  assertEqual(1, strscpy(dest3, source, 9));
  assertEqual("stringte", dest3);
  assertEqual('\0', dest3[8]);
  assertEqual('9', dest3[9]);
  assertEqual('a', dest3[10]);
  assertEqual('\0', dest3[13]);
  assertEqual(0, strscpy(dest4, source, sizeof(dest4)));
  assertEqual("stringtest", dest4);
  assertEqual('\0', dest4[10]);
  assertEqual('b', dest4[11]);
  assertEqual('\0', dest4[13]);
}

unittest(stringCopy_P) {
  static const char source[11] PROGMEM = "stringtest";
  char dest1[9];
  char dest2[] = "012345678";
  char dest3[] = "0123456789abc";
  char dest4[] = "0123456789abc";

  assertEqual(1, strscpy_P(dest1, source, sizeof(dest1)));
  assertEqual("stringte", dest1);
  assertEqual('\0', dest1[8]);
  assertEqual(1, strscpy_P(dest2, source, sizeof(dest2)));
  assertEqual("stringtes", dest2);
  assertEqual('\0', dest2[9]);
  assertEqual(1, strscpy_P(dest3, source, 9));
  assertEqual("stringte", dest3);
  assertEqual('\0', dest3[8]);
  assertEqual('9', dest3[9]);
  assertEqual('a', dest3[10]);
  assertEqual('\0', dest3[13]);
  assertEqual(0, strscpy_P(dest4, source, sizeof(dest4)));
  assertEqual("stringtest", dest4);
  assertEqual('\0', dest4[10]);
  assertEqual('b', dest4[11]);
  assertEqual('\0', dest4[13]);
}

unittest(floatToString) {
  String serialOutput;

  double num = 1000.5;
  char buffer[7];
  int error_code;
  error_code = floattostrf(num, 6, 1, buffer, sizeof(buffer));
  assertEqual(0, error_code);
  assertEqual("1000.5", buffer);

  num = 10.4;
  error_code = floattostrf(num, 6, 1, buffer, sizeof(buffer));
  assertEqual(0, error_code);
  assertEqual("  10.4", buffer);

  num = 10.444;
  error_code = floattostrf(num, 6, 1, buffer, sizeof(buffer));
  assertEqual(0, error_code);
  assertEqual("  10.4", buffer);

  num = -1.444;
  error_code = floattostrf(num, 6, 2, buffer, sizeof(buffer));
  assertEqual(0, error_code);
  assertEqual(" -1.44", buffer);

  num = -10.444;
  error_code = floattostrf(num, 6, 2, buffer, sizeof(buffer));
  assertEqual(0, error_code);
  assertEqual("-10.44", buffer);

  state->serialPort[0].dataOut = "";
  num = -1000.444;
  error_code = floattostrf(num, 6, 2, buffer, sizeof(buffer));
  assertEqual(1, error_code);
  assertEqual("-1000.", buffer);
  serialOutput = state->serialPort[0].dataOut;
  int16_t j = serialOutput.indexOf('\r');
  // assertEqual("WARNING! String \"-1000.44\" was truncated to \"-1000.\"\r\n", serialOutput.c_str());
  assertEqual("WARNING! String \"-1000.44\" was truncated to \"-1000.\"", serialOutput.substring(0, j).c_str());
  COUT << serialOutput.c_str();

  // String contents(data), line;
  // int16_t i = contents.indexOf('\n');
  // line = contents.substring(0, i);
  // assertEqual("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd", line.c_str());

  state->serialPort[0].dataOut = "";
  num = 1.3;
  error_code = floattostrf(num, 6, 5, buffer, sizeof(buffer));
  assertEqual(1, error_code);
  assertEqual("1.3000", buffer);
  serialOutput = state->serialPort[0].dataOut;
  assertEqual("WARNING! String \"1.30000\" was truncated to \"1.3000\"\r\n", serialOutput.c_str());
  COUT << serialOutput.c_str();

  state->serialPort[0].dataOut = "";
  num = 10000000000000.0;
  error_code = floattostrf(num, 6, 1, buffer, sizeof(buffer));
  assertEqual(2, error_code);
  assertEqual("100000", buffer);
  serialOutput = state->serialPort[0].dataOut;
  assertEqual("WARNING! Overflow may have occurred before truncating to \"100000\"\r\n", serialOutput.c_str());
  COUT << serialOutput.c_str();
}

unittest_main()
