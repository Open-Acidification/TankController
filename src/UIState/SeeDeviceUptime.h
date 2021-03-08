/**
 * SeeDeviceUptime.h
 *
 * Display the device uptime & Current time
 */
#pragma once
#include "UIState.h"

class SeeDeviceUptime : public UIState {
public:
  void handleKey(char key);
  const char *prompt() {
    return "Device time";
  };
};
