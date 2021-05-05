#include <RTClib.h>
#include <Serial_TC.h>

#include "ArduinoUnitTests.h"
#include "Devices/DateTime_TC.h"
#include "Devices/Serial_TC.h"
#include "TC_util.h"

unittest(SerialTest) {
  GodmodeState* state = GODMODE();
  DateTime_TC::now();                 // this puts stuff on the serial port that we want to ignore
  serial("foo");                      // "Unable to create directory"
  state->serialPort[0].dataIn = "";   // the queue of data waiting to be read
  state->serialPort[0].dataOut = "";  // the history of data written

  // basic serial output
  serial("abc %s %i %.4f", "XYZ", 42, 1.375);
  assertEqual("", state->serialPort[0].dataIn);
  assertEqual("abc XYZ 42 1.3750\r\n", state->serialPort[0].dataOut);

  // serial output with timestamp
  state->serialPort[0].dataOut = "";  // the history of data written
  serialWithTime("foo");
  assertEqual("Timestamp of next line: 202", state->serialPort[0].dataOut.substr(0, 27));
  assertEqual(".000\r\nfoo\r\n", state->serialPort[0].dataOut.substr(27 + 16));
}

unittest_main()
