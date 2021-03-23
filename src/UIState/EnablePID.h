/**
 * EnablePID.h
 *
 * Enable PID Controller
 */
#pragma once
#include "UIState.h"

class EnablePID : public UIState {
public:
  EnablePID(TankControllerLib* tc) : UIState(tc) {
  }
  void handleKey(char key);
  String name() {
    return "EnablePID";
  }
  const char* prompt() {
    return "Enable PID?     ";
  };
};
