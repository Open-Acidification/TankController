#include "MainMenu.h"

#include "CalibrationManagement.h"
#include "Devices/TempProbe_TC.h"
#include "EnablePID.h"
#include "PHCalibration.h"
#include "PIDTuningMenu.h"
#include "ResetLCDScreen.h"
#include "SeeDeviceAddress.h"
#include "SeeDeviceUptime.h"
#include "SeePIDConstants.h"
#include "SeeTankID.h"
#include "SetChillOrHeat.h"
#include "SetGoogleSheetInterval.h"
#include "SetPHSetPoint.h"
#include "SetTankID.h"
#include "SetTempSetPoint.h"
#include "SetTime.h"
#include "TemperatureCalibration.h"

/**
 * Branch to other states to handle various menu options
 */
void MainMenu::handleKey(char key) {
  switch (key) {
    case 'A':  // Set pH set_point
      this->setNextState((UIState *)new SetPHSetPoint(tc));
      break;
    case 'B':  // Set Temperature set_point
      this->setNextState((UIState *)new SetTempSetPoint(tc));
      break;
    case 'C':  // pH Calibration
      this->setNextState((UIState *)new PHCalibration(tc));
      break;
    case 'D':  // Calibration Management
      this->setNextState((UIState *)new CalibrationManagement(tc));
      break;
    case '#':  // Set Tank ID
      this->setNextState((UIState *)new SetTankID(tc));
      break;
    case '*':  // Set Google Sheet Interval
      this->setNextState((UIState *)new SetGoogleSheetInterval(tc));
      break;
    case '0':  // See Device Uptime & Current Time
      this->setNextState((UIState *)new SeeDeviceUptime(tc));
      break;
    case '1':  // See Device addresses
      this->setNextState((UIState *)new SeeDeviceAddress(tc));
      break;
    case '2':  // Reset LCD Screen
      this->setNextState((UIState *)new ResetLCDScreen(tc));
      break;
    case '3':  // See Tank ID and Log File Name
      this->setNextState((UIState *)new SeeTankID(tc));
      break;
    case '4':  // See PID Constants
      this->setNextState((UIState *)new SeePIDConstants(tc));
      break;
    case '5':  // PID Tuning
      this->setNextState((UIState *)new PIDTuningMenu(tc));
      break;
    case '6':  // Temperature Calibration
      this->setNextState((UIState *)new TemperatureCalibration(tc));
      break;
    case '7':  // Manual Set Time
      this->setNextState((UIState *)new SetTime(tc));
      break;
    case '8':  // Enable PID
      this->setNextState((UIState *)new EnablePID(tc));
      break;
    case '9':  // Set Chill or Heat
      this->setNextState((UIState *)new SetChillOrHeat(tc));
      break;
    default:
      returnToMainMenu();
      break;
  }
}

// show current temp and pH
void MainMenu::loop() {
  TempProbe_TC *tempProbe = TempProbe_TC::instance();
  char output[17];
  sprintf(output, "Temp=%2.2f", tempProbe->getRunningAverage());
  LiquidCrystal_TC::instance()->writeLine(output, 1);
}
