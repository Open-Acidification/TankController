/**
 * Abstract superclass for the NumberCollector UI states
 */

#pragma once
#include "UIState.h"

class NumberCollector : public UIState {
public:
  virtual void handleKey(char key);
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
  virtual bool showCurrentValue() {
    return true;
  }

  float value = 0.0;
  uint16_t numDigits = 0;
  uint16_t factor = 10;
  bool hasDecimal = false;
};

#if defined(ARDUINO_CI_COMPILATION_MOCKS)
class TestNumberCollector : public NumberCollector {
public:
  // Implementation
  void setValue(float value) {
    storedValue = value;
  }
  const __FlashStringHelper* name() {
    return F("TestNumberCollector");
  }
  float getCurrentValue() {
    return priorValue;
  }
  uint16_t getCurrentValuePrecision() {
    return priorValuePrecision;
  }
  const __FlashStringHelper* prompt() {
    return F("Test:");
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

class TestIntNumberCollector : public TestNumberCollector {
public:
  virtual bool isInteger() {
    return true;
  }
};

class TestNumberCollectorWithNoCurrentValue : public TestNumberCollector {
public:
  bool showCurrentValue() {
    return false;
  }
};

#endif
