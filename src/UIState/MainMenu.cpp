/*
 * MainMenu.cpp
 *
 * Branch to other states to handle various menu options
 */

// #include "CalibrationManagement.h"
// #include "ChangeGoogleSheetInterval.h"
// #include "ChangeTankID.h"
// #include "EnablePID.h"
// #include "ManualSetTime.h"
// #include "PHCalibration.h"
// #include "PIDTuningMenu.h"
// #include "ResetLCDScreen.h"
// #include "SeeDeviceUptime.h"
// #include "SeePIDConstants.h"
// #include "SeeTankID.h"
// #include "SetChillOrHeat.h"
#include "SetPHSetPoint.h"
// #include "SetTempSetPoint.h"
// #include "TemperatureCalibration.h"
#include "MainMenu.h"

void MainMenu::handleKey(char key) {
  switch (key) {
    case 'A':  // Change pH set_point
      this->changeState((UIState*)new SetPHSetPoint);
      break;
    case 'B':  // Change Temperature set_point
      // this->changeState((UIState*) new SetTempSetPoint);
      break;
    case 'C':  // pH Calibration
      // this->changeState((UIState*) new PHCalibration);
      break;
    case 'D':  // Calibration Management
      // this->changeState((UIState*) new CalibrationManagement);
      break;
    case '#':  // Change Tank ID
      // this->changeState((UIState*) new ChangeTankID);
      break;
    case '*':  // Change Google Sheet Interval
      // this->changeState((UIState*) new ChangeGoogleSheetInterval);
      break;
    case '0':  // See Device Uptime & Current Time
      // this->changeState((UIState*) new SeeDeviceUptime);
      break;
    case '1':  // See Device addresses
      // this->changeState((UIState*) new SeeDeviceAddresses);
      break;
    case '2':  // Reset LCD Screen
      // this->changeState((UIState*) new ResetLCDScreen);
      break;
    case '3':  // See Tank ID and Log File Name
      // this->changeState((UIState*) new SeeTankID);
      break;
    case '4':  // See PID Constants
      // this->changeState((UIState*) new SeePIDConstants);
      break;
    case '5':  // PID Tuning Menu
      // this->changeState((UIState*) new PIDTuningMenu);
      break;
    case '6':  // Temperature Calibration
      // this->changeState((UIState*) new TemperatureCalibration);
      break;
    case '7':  // Manual Set Time
      // this->changeState((UIState*) new ManualSetTime);
      break;
    case '8':  // Enable PID
      // this->changeState((UIState*) new EnablePID);
      break;
    case '9':  // Set Chill or Heat
      // this->changeState((UIState*) new SetChillOrHeat);
      break;
    default:
      changeState((UIState*)new MainMenu);
      break;
  }
}
