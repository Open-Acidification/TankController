/**
 * SeeGoogleMins.h
 *
 * Display the Google Sheet Interval
 */
#pragma once
#include "UIState.h"

class SeeGoogleMins : public UIState {
public:
  SeeGoogleMins(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  const char* name() {
    return (const char*)F("SeeGoogleMins");
  }
  const char* prompt() {
    return (const char*)F("Google Mins:");
  };
};
