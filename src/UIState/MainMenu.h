/**
 * MainMenu.h
 *
 * Branch to other states to handle various menu options
 */

#pragma once

#include "TC_util.h"
#include "UIState.h"

class TankControllerLib;  // forward reference
class MainMenu : public UIState {
public:
  enum ViewCommands {
    VIEW_TIME,
    VIEW_PID,
    VIEW_PH_SLOPE,
    VIEW_TANK_ID,
    VIEW_LOG_FILE,
    VIEW_GOOGLE_MINS,
    VIEW_ADDRESS,
    VIEW_VERSION,
    VIEW_FREE_MEMORY,
    VIEW_COMMAND_COUNT  // always last
  };
  enum SetCommands {
    SET_PH,
    SET_TEMPERATURE,
    SET_CALIBRATION,
    SET_CALIBRATION_CLEAR,
    SET_TEMP_CALIBRATION,
    SET_KP,
    SET_KI,
    SET_KD,
    SET_PID_ON_OFF,
    SET_CHILL_OR_HEAT,
    SET_GOOGLE_MINS,
    SET_TIME,
    SET_TANK_ID,
    SET_COMMAND_COUNT  // always last
  };
  MainMenu(TankControllerLib* tc);
  void handleKey(char key);
  const char* name() {
    return CSTR("MainMenu");
  }
  const char* prompt() {
    return CSTR("Main Menu");
  };
  bool isMainMenu() {
    return true;
  }
  void loop();

private:
  int16_t level1 = 0;
  int16_t level2 = -1;
  const char* viewMenus[VIEW_COMMAND_COUNT];
  const char* setMenus[SET_COMMAND_COUNT];
  void left();
  void right();
  void up();
  void down();
  void selectView();
  void selectSet();
  void idle();
};
