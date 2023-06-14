#include <RTClib.h>
#include <Serial_TC.h>

#include "ArduinoUnitTests.h"
#include "Devices/DateTime_TC.h"
#include "Devices/Serial_TC.h"
#include "TC_util.h"
#include "TankController.h"

unittest(SerialTest) {
  GodmodeState* state = GODMODE();
  DateTime_TC::now();                 // this puts stuff on the serial port that we want to ignore
  serial(F("foo"));                   // "Unable to create directory"
  state->serialPort[0].dataIn = "";   // the queue of data waiting to be read
  state->serialPort[0].dataOut = "";  // the history of data written

  // basic serial output
  serial(F("abc %s %i %.4f"), "XYZ", 42, 1.375);
  assertEqual("", state->serialPort[0].dataIn);
  assertEqual("abc XYZ 42 1.3750\r\n", state->serialPort[0].dataOut);
}

unittest(report_loop_delay) {
  GodmodeState* state = GODMODE();
  TankController* tc = TankController::instance();
  DateTime_TC::now();                 // this puts stuff on the serial port that we want to ignore
  serial(F("foo"));                   // "Unable to create directory"
  state->serialPort[0].dataIn = "";   // the queue of data waiting to be read
  state->serialPort[0].dataOut = "";  // the history of data written

  // no report if no delay
  tc->loop();  // for SD log
  tc->loop();  // for serial log
  delay(450);
  state->serialPort[0].dataOut = "";  // the history of data written
  tc->loop();                         // for SD log
  tc->loop();                         // for serial log
  assertEqual("", state->serialPort[0].dataIn);
  assertEqual("", state->serialPort[0].dataOut);

  delay(505);
  tc->loop();
  assertEqual("", state->serialPort[0].dataIn);
  assertEqual("unexpected delay of 512 ms\r\n", state->serialPort[0].dataOut);
}

unittest_main()
