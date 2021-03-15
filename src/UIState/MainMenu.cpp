#include "MainMenu.h"

#include "CalibrationManagement.h"
#include "ChangeGoogleSheetInterval.h"
#include "ChangeTankID.h"
#include "EnablePID.h"
#include "ManualSetTime.h"
#include "PHCalibration.h"
#include "PIDTuningMenu.h"
#include "ResetLCDScreen.h"
#include "SeeDeviceUptime.h"
#include "SeePIDConstants.h"
#include "SeeTankID.h"
#include "SetChillOrHeat.h"
#include "SetPHSetPoint.h"
#include "SetTempSetPoint.h"
#include "TemperatureCalibration.h"

/**
 * Branch to other states to handle various menu options
 */
void MainMenu::handleKey(char key) {
  switch (key) {
    case 'A':  // Change pH set_point
      this->setNextState((UIState*)new SetPHSetPoint(tc));
      break;
    case 'B':  // Change Temperature set_point
      this->setNextState((UIState*)new SetTempSetPoint(tc));
      break;
    case 'C':  // pH Calibration
      // this->setNextState((UIState*) new PHCalibration);
      break;
    case 'D':  // Calibration Management
      // this->setNextState((UIState*) new CalibrationManagement);
      break;
    case '#':  // Change Tank ID
      // this->setNextState((UIState*) new ChangeTankID);
      break;
    case '*':  // Change Google Sheet Interval
      // this->setNextState((UIState*) new ChangeGoogleSheetInterval);
      break;
    case '0':  // See Device Uptime & Current Time
      // this->setNextState((UIState*) new SeeDeviceUptime);
      break;
    case '1':  // See Device addresses
      // this->setNextState((UIState*) new SeeDeviceAddresses);
      break;
    case '2':  // Reset LCD Screen
      // this->setNextState((UIState*) new ResetLCDScreen);
      break;
    case '3':  // See Tank ID and Log File Name
      // this->setNextState((UIState*) new SeeTankID);
      break;
    case '4':  // See PID Constants
      // this->setNextState((UIState*) new SeePIDConstants);
      break;
    case '5':  // PID Tuning Menu
      // this->setNextState((UIState*) new PIDTuningMenu);
      break;
    case '6':  // Temperature Calibration
      // this->setNextState((UIState*) new TemperatureCalibration);
      break;
    case '7':  // Manual Set Time
      // this->setNextState((UIState*) new ManualSetTime);
      break;
    case '8':  // Enable PID
      // this->setNextState((UIState*) new EnablePID);
      break;
    case '9':  // Set Chill or Heat
      // this->setNextState((UIState*) new SetChillOrHeat);
      break;
    default:
      returnToMainMenu();
      break;
  }
}
