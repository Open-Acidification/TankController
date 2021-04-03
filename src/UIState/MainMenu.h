/**
 * MainMenu.h
 *
 * Branch to other states to handle various menu options
 */

#pragma once

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
    VIEW_IP_ADDRESS,
    VIEW_MAC_ADDRESS,
    VIEW_VERSION,
    VIEW_UPTIME,
    VIEW_COMMAND_COUNT
  };
  enum SetCommands {
    SET_PH,
    SET_TEMPERATURE,
    SET_CALIBRATION_1,
    SET_CALIBRATION_2,
    SET_CALIBRATION_CLEAR,
    SET_TEMP_CALIBRATION,
    SET_PID_AUTO_TUNE,
    SET_PID_MANUAL_TUNE,
    SET_PID_ON_OFF,
    SET_CHILL_OR_HEAT,
    SET_GOOGLE_MINS,
    SET_TIME,
    SET_TANK_ID,
    SET_COMMAND_COUNT
  };
  MainMenu(TankControllerLib* tc);
  void handleKey(char key);
  const char* name() {
    return "MainMenu";
  }
  const char* prompt() {
    return "Main Menu       ";
  };
  bool isMainMenu() {
    return true;
  }
  void loop();

private:
  int level1 = 0;
  int level2 = -1;
  String viewMenus[VIEW_COMMAND_COUNT];
  String setMenus[SET_COMMAND_COUNT];
  void left();
  void right();
  void up();
  void down();
  void selectView();
  void selectSet();
  void idle();
};
