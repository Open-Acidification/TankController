#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include <iostream>

#include "PushingBox.h"
#include "TC_util.h"
#include "TankController.h"

GodmodeState* state = GODMODE();
const uint16_t LOG_SIZE = 10;
struct {
  long time;
  bool value;
} pinLog[LOG_SIZE];
uint16_t logIndex = 0;
bool overflowFlag = false;
const uint16_t LED_PIN = 13;

class BitCollector : public DataStreamObserver {
public:
  BitCollector() : DataStreamObserver(false, false) {
  }
  virtual void onBit(bool aBit) {
    if (logIndex < LOG_SIZE) {
      pinLog[logIndex].time = millis();
      pinLog[logIndex].value = state->digitalPin[LED_PIN];
      COUT(logIndex << ":" << pinLog[logIndex].time << ":" << pinLog[logIndex].value);
      logIndex++;
    } else {
      overflowFlag = true;
    }
  }

  virtual String observerName() const {
    return "BitCollector";
  }
};

unittest(loop) {
  BitCollector led;
  logIndex = 0;
  TankController* tank = TankController::instance();
  PushingBox::instance();
  state->digitalPin[LED_PIN].addObserver("led", &led);
  tank->setup();
  state->resetClock();
  assertEqual(0, millis());
  tank->loop();
  assertTrue(0 <= millis() && millis() <= 100);
  delay(1000);
  assertTrue(1000 <= millis() && millis() <= 1100);
  tank->loop();
  delay(1000);
  tank->loop();
  delay(1000);
  tank->loop();
  delay(1000);
  state->digitalPin[LED_PIN].removeObserver("led");
  assertFalse(overflowFlag);
  assertEqual(4, logIndex);
  assertEqual(0, pinLog[0].time);
  assertEqual(HIGH, pinLog[0].value);
  assertTrue(1000 <= pinLog[1].time && pinLog[1].time <= 1100);
  assertEqual(LOW, pinLog[1].value);
  assertTrue(2000 <= pinLog[2].time && pinLog[2].time <= 2100);
  assertEqual(HIGH, pinLog[2].value);
  assertTrue(3000 <= pinLog[3].time && pinLog[3].time <= 3100);
  assertEqual(LOW, pinLog[3].value);
}

unittest_main()
