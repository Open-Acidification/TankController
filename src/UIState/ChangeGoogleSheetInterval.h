/**
 * ChangeGoogleSheetInterval.h
 *
 * Change Google Sheet Interval
 */
#pragma once
#include "UIState.h"

class ChangeGoogleSheetInterval : public UIState {
public:
  void handleKey(char key);
  const char *prompt() {
    return "G Sheet Interval";
  };
};
