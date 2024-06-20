/**
 * SeePh.h
 *
 * See PID Constants
 */
#pragma once
#include "UIState.h"
#include "model/TC_util.h"

class SeePh : public UIState {
public:
  void loop();
  const __FlashStringHelper *name() {
    return F("SeePh");
  }
  void start() {
    startTime = millis();
  }

private:
  void loadHeader(uint16_t line);
  void loadPHFunctionType(uint16_t line);
  void loadTypeVariables(uint16_t line);
  void loadValues(uint16_t line);

  uint32_t startTime;
};
