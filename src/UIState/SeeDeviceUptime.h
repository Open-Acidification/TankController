/**
 * SeeDeviceUptime.h
 *
 * Display the device uptime & Current time
 */
#pragma once
#include "UIState.h"

class SeeDeviceUptime : public UIState {
public:
  SeeDeviceUptime(TankController* tc) : UIState(tc) {
  }
  void loop();
  const __FlashStringHelper* name() {
    return F("SeeDeviceUptime");
  }
};
