/**
 * SetChillOrHeat.h
 *
 * Set if the chiller or heater is used
 */
#pragma once
#include "NumberCollector.h"

class SetChillOrHeat : public NumberCollector {
public:
  virtual bool isInteger() {
    return true;
  }
  const __FlashStringHelper* name() {
    return F("SetChillOrHeat");
  }
  float getCurrentValue();
  const __FlashStringHelper* prompt() {
    return F("1:Chill; 9:Heat");
  };
  void setValue(float value);
};
