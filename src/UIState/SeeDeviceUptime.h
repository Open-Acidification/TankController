/**
 * SeeDeviceUptime.h
 *
 * Display the device uptime & Current time
 */
#pragma once
#include "UIState.h"

class SeeDeviceUptime : public UIState {
public:
  SeeDeviceUptime(TankControllerLib* tc) : UIState(tc) {
  }
  void loop();
  const char* name() {
    return "SeeDeviceUptime";
  }
  void start() {
    msEnd = millis() + 5000;
  }

private:
  int msEnd;
};
