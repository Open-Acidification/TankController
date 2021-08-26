/**
 * Abstract superclass for the NumberCollector UI states
 */

#pragma once
#include "TC_util.h"
#include "UIState.h"

class NumCollectorState : public UIState {
public:
  NumCollectorState(TankControllerLib* tc) : UIState(tc) {
  }
  void handleKey(char key);
  virtual void setValue(float value) = 0;
  virtual void start();

protected:
  // Helper Functions
  void backSpace();
  void clear();
  void handleDigit(uint16_t digit);
  virtual bool isInteger() {
    return false;
  }
  void printValue();
  virtual float getCurrentValue() = 0;
  virtual uint16_t getCurrentValuePrecision() {
    return 0;
  };

  float value = 0.0;
  uint16_t numDigits = 0;
  uint16_t factor = 10;
  bool hasDecimal = false;
};

#ifdef MOCK_PINS_COUNT
class TestNumCollectorState : public NumCollectorState {
public:
  TestNumCollectorState(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  // Implementation
  void setValue(float value) {
    storedValue = value;
  }
  const char* name() {
    return CSTR("TestNumCollectorState");
  }
  float getCurrentValue() {
    return priorValue;
  }
  uint16_t getCurrentValuePrecision() {
    return priorValuePrecision;
  }
  const char* prompt() {
    return CSTR("Test:");
  }

  // Testing
  float getValue() {
    return value;
  }
  float getStoredValue() {
    return storedValue;
  };
  void setPriorValue(float aValue) {
    priorValue = aValue;
  }
  void setPriorValuePrecision(uint16_t aValue) {
    priorValuePrecision = aValue;
  }

private:
  float storedValue = 0.0;
  float priorValue = 0.0;
  uint16_t priorValuePrecision = 0;
};

class TestIntNumCollectorState : public TestNumCollectorState {
public:
  TestIntNumCollectorState(TankControllerLib* tc) : TestNumCollectorState(tc) {
  }
  virtual bool isInteger() {
    return true;
  }
};

#endif
