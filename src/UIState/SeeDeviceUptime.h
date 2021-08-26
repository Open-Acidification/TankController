/**
 * SeeDeviceUptime.h
 *
 * Display the device uptime & Current time
 */
#pragma once
#include "TC_util.h"
#include "UIState.h"

class SeeDeviceUptime : public UIState {
public:
  SeeDeviceUptime(TankControllerLib* tc) : UIState(tc) {
  }
  void loop();
  const char* name() {
    return CSTR("SeeDeviceUptime");
  }
};
