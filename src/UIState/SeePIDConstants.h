/**
 * SeePIDConstants.h
 *
 * See PID Constants
 */
#pragma once
#include "TC_util.h"
#include "UIState.h"

class SeePIDConstants : public UIState {
public:
  SeePIDConstants(TankController* tc) : UIState(tc) {
  }
  void loop() override;
  const __FlashStringHelper* name() {
    return F("SeePIDConstants");
  }
  void start() override {
    startTime = millis();
  }
  int writeToBuffer(char*, int, int) override;

private:
  void loadKp(uint16_t line);
  void loadKi(uint16_t line);
  void loadKd(uint16_t line);
  void loadPID(uint16_t line);

  uint32_t startTime;
};
