/**
 * SeeTemperature.h
 *
 * See Temperature
 */
#pragma once
#include "TC_util.h"
#include "UIState.h"

class SeeTemperature : public UIState {
public:
  enum tempSetTypeTypes {
    FLAT_TYPE,
    RAMP_TYPE,
    SINE_TYPE,
  };

  SeeTemperature(TankController *tc) : UIState(tc) {
  }
  void loop();
  const __FlashStringHelper *name() {
    return F("SeeTemperature");
  }
  void start() {
    startTime = millis();
  }

private:
  void loadHeader(uint16_t line);
  void loadTempSetType(uint16_t line);
  void loadTypeVariables(uint16_t line);
  void loadValues(uint16_t line);

  uint32_t startTime;
};
