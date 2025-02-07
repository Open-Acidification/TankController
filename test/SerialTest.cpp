#include <RTClib.h>
#include <Serial_TC.h>

#include "ArduinoUnitTests.h"
#include "DateTime_TC.h"
#include "Serial_TC.h"
#include "TC_util.h"
#include "TankController.h"

GodmodeState* state = GODMODE();
TankController* tc = TankController::instance();
Serial_TC* serialPort = Serial_TC::instance();

unittest_setup() {
  state->resetClock();
  DateTime_TC d1(2023, 8, 15);
  d1.setAsCurrent();
  serialPort->clearBuffer();
}

unittest(SerialTest) {
  serial(F("abc %s %i %.4f"), "XYZ", 42, 1.375);
  assertEqual("abc XYZ 42 1.3750", serialPort->getBuffer());
}

unittest(report_loop_delay) {
  delay(5);
  tc->loop(true);
  int oldLoopDelayCount = tc->loopDelayCount;
  delay(505);
  tc->loop(true);
  assertEqual(1, tc->loopDelayCount - oldLoopDelayCount);
}

unittest_main()
