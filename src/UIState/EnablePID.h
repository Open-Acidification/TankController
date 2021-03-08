/**
 * EnablePID.h
 *
 * Enable PID Controller
 */
#pragma once
#include "UIState.h"

class EnablePID : public UIState {
public:
  void handleKey(char key);
  const char *prompt() {
    return "Enable PID?";
  };
};
