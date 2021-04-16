#include <RTClib.h>
#include <Serial_TC.h>

#include "ArduinoUnitTests.h"
#include "Devices/Serial_TC.h"

unittest(SerialTest) {
  GodmodeState* state = GODMODE();
  state->serialPort[0].dataIn = "";   // the queue of data waiting to be read
  state->serialPort[0].dataOut = "";  // the history of data written

  Serial_TC* mySerial = Serial_TC::instance();

  state->serialPort[0].dataOut = "";  // the history of data written
  mySerial->printf("abc %s %i %.4f", "XYZ", 42, 1.375);
  assertEqual("", state->serialPort[0].dataIn);
  assertEqual("abc XYZ 42 1.3750\r\n", state->serialPort[0].dataOut);
}

unittest_main()
