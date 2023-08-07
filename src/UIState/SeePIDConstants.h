/**
 * SeePIDConstants.h
 *
 * See PID Constants
 */
#pragma once
#include "UIState.h"
#include "model/TC_util.h"

class SeePIDConstants : public UIState {
public:
  void loop() override;
  const __FlashStringHelper* name() override {
    return F("SeePIDConstants");
  }
  void start() override {
    startTime = millis();
  }

private:
  void loadKp(uint16_t line);
  void loadKi(uint16_t line);
  void loadKd(uint16_t line);
  void loadPID(uint16_t line);

  uint32_t startTime;
};
