#include "MainMenu.h"

#include "CalibrationManagement.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "Devices/PHProbe.h"
#include "Devices/TempProbe_TC.h"
#include "Devices/TemperatureControl.h"
#include "EnablePID.h"
#include "PHCalibrationMid.h"
#include "ResetLCDScreen.h"
#include "SeeDeviceAddress.h"
#include "SeeDeviceUptime.h"
#include "SeeGoogleMins.h"
#include "SeeLogFile.h"
#include "SeePHSlope.h"
#include "SeePIDConstants.h"
#include "SeeTankID.h"
#include "SeeVersion.h"
#include "SetCalibrationClear.h"
#include "SetChillOrHeat.h"
#include "SetGoogleSheetInterval.h"
#include "SetKD.h"
#include "SetKI.h"
#include "SetKP.h"
#include "SetPHSetPoint.h"
#include "SetTankID.h"
#include "SetTempSetPoint.h"
#include "SetTime.h"
#include "TemperatureCalibration.h"

MainMenu::MainMenu(TankControllerLib *tc) : UIState(tc) {
  viewMenus[VIEW_GOOGLE_MINS] = String("View Google mins");
  viewMenus[VIEW_IP_ADDRESS] = String("View IP address ");
  viewMenus[VIEW_LOG_FILE] = String("View log file   ");
  viewMenus[VIEW_MAC_ADDRESS] = String("View MAC address");
  viewMenus[VIEW_PID] = String("View PID        ");
  viewMenus[VIEW_PH_SLOPE] = String("View pH slope   ");
  viewMenus[VIEW_TANK_ID] = String("View tank ID    ");
  viewMenus[VIEW_TIME] = String("View time       ");
  viewMenus[VIEW_UPTIME] = String("View uptime     ");
  viewMenus[VIEW_VERSION] = String("View version    ");

  setMenus[SET_CALIBRATION] = String("pH calibration ");
  setMenus[SET_CALIBRATION_CLEAR] = String("Clear pH calibra");
  setMenus[SET_CHILL_OR_HEAT] = String("Set chill/heat  ");
  setMenus[SET_GOOGLE_MINS] = String("Set Google mins ");
  setMenus[SET_PH] = String("Set pH target   ");
  setMenus[SET_KD] = String("Set KD          ");
  setMenus[SET_KI] = String("Set KI          ");
  setMenus[SET_KP] = String("Set KP          ");
  setMenus[SET_PID_ON_OFF] = String("PID on/off      ");
  setMenus[SET_TANK_ID] = String("Set Tank ID     ");
  setMenus[SET_TEMP_CALIBRATION] = String("Temp calibration");
  setMenus[SET_TEMPERATURE] = String("Set temperature ");
  setMenus[SET_TIME] = String("Set date/time   ");
}

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
    case 'D':  // Reset
      level1 = 0;
      level2 = -1;
      break;
    case '2':  // up
      up();
      break;
    case '4':  // left
      left();
      break;
    case '6':  // right
      right();
      break;
    case '8':  // down
      down();
      break;
    default:
      // ignore invalid keys
      break;
  }
}

void MainMenu::left() {
  if (level2 == -1) {
    level1 = 0;
  } else {
    level2 = -1;
  }
}

void MainMenu::right() {
  if (level1 == 0) {
    level1 = 1;
    level2 = -1;
  } else if (level2 == -1) {
    level2 = 0;
  } else if (level1 == 1) {
    selectView();
  } else {
    selectSet();
  }
}

void MainMenu::up() {
  if (level2 == -1) {
    level1 = (level1 + 2) % 3;
  } else {
    if (level1 == 1) {
      level2 = (level2 + VIEW_COMMAND_COUNT - 1) % VIEW_COMMAND_COUNT;
    } else {
      level2 = (level2 + SET_COMMAND_COUNT - 1) % SET_COMMAND_COUNT;
    }
  }
}

void MainMenu::down() {
  if (level2 == -1) {
    level1 = (level1 + 1) % 3;
  } else {
    if (level1 == 1) {
      level2 = (level2 + 1) % VIEW_COMMAND_COUNT;
    } else {
      level2 = (level2 + 1) % SET_COMMAND_COUNT;
    }
  }
}

void MainMenu::selectView() {
  switch (level2) {
    case VIEW_GOOGLE_MINS:
      this->setNextState(static_cast<UIState *>(new SeeGoogleMins(tc)));
      break;
    case VIEW_IP_ADDRESS:
      this->setNextState((UIState *)new SeeDeviceAddress(tc));
      break;
    case VIEW_LOG_FILE:
      this->setNextState(static_cast<UIState *>(new SeeLogFile(tc)));
      break;
    case VIEW_MAC_ADDRESS:
      this->setNextState((UIState *)new SeeDeviceAddress(tc));
      break;
    case VIEW_PID:
      this->setNextState((UIState *)new SeePIDConstants(tc));
      break;
    case VIEW_PH_SLOPE:
      this->setNextState((UIState *)new SeePHSlope(tc));
      break;
    case VIEW_TANK_ID:
      this->setNextState((UIState *)new SeeTankID(tc));
      break;
    case VIEW_TIME:
      this->setNextState((UIState *)new SeeDeviceUptime(tc));
      break;
    case VIEW_UPTIME:
      this->setNextState((UIState *)new SeeDeviceUptime(tc));
      break;
    case VIEW_VERSION:
      this->setNextState((UIState *)new SeeVersion(tc));
      break;
    default:
      break;
  }
}

void MainMenu::selectSet() {
  switch (level2) {
    case SET_CALIBRATION:
      this->setNextState((UIState *)new PHCalibrationMid(tc));
      break;
    case SET_CALIBRATION_CLEAR:
      this->setNextState((UIState *)new SetCalibrationClear(tc));
      break;
    case SET_CHILL_OR_HEAT:
      this->setNextState((UIState *)new SetChillOrHeat(tc));
      break;
    case SET_GOOGLE_MINS:
      this->setNextState((UIState *)new SetGoogleSheetInterval(tc));
      break;
    case SET_PH:
      this->setNextState((UIState *)new SetPHSetPoint(tc));
      break;
    case SET_KD:
      this->setNextState((UIState *)new SetKD(tc));
      break;
    case SET_KI:
      this->setNextState((UIState *)new SetKI(tc));
      break;
    case SET_KP:
      this->setNextState((UIState *)new SetKP(tc));
      break;
    case SET_PID_ON_OFF:
      this->setNextState(static_cast<UIState *>(new EnablePID(tc)));
      break;
    case SET_TANK_ID:
      this->setNextState((UIState *)new SetTankID(tc));
      break;
    case SET_TEMP_CALIBRATION:
      this->setNextState((UIState *)new TemperatureCalibration(tc));
      break;
    case SET_TEMPERATURE:
      this->setNextState((UIState *)new SetTempSetPoint(tc));
      break;
    case SET_TIME:
      this->setNextState((UIState *)new SetTime(tc));
      break;
    default:
      break;
  }
}

// show current temp and pH
void MainMenu::idle() {
  char output[17];
  snprintf(output, sizeof(output), "pH=%01.3f   %1.3f", PHProbe::instance()->getPh(),
           PHControl::instance()->getTargetPh());
  LiquidCrystal_TC::instance()->writeLine(output, 0);
  TemperatureControl *tempControl = TemperatureControl::instance();
  TempProbe_TC *tempProbe = TempProbe_TC::instance();
  double temp = tempProbe->getRunningAverage();
  if (temp < 0.0) {
    temp = 0.0;
  } else if (99.99 < temp) {
    temp = 99.99;
  }
  snprintf(output, sizeof(output), "T=%02.2f  %c %2.2f", temp, (tempControl->isHeater() ? 'H' : 'C'),
           tempControl->getTargetTemperature());
  LiquidCrystal_TC::instance()->writeLine(output, 1);
}

void MainMenu::loop() {
  if (level1 == 0) {
    idle();
  } else {
    if (level1 == 1) {
      if (level2 == -1) {
        LiquidCrystal_TC::instance()->writeLine("View TC settings", 0);
      } else {
        LiquidCrystal_TC::instance()->writeLine(viewMenus[level2].c_str(), 0);
      }
    } else {
      if (level2 == -1) {
        LiquidCrystal_TC::instance()->writeLine("Change settings ", 0);
      } else {
        LiquidCrystal_TC::instance()->writeLine(setMenus[level2].c_str(), 0);
      }
    }
    LiquidCrystal_TC::instance()->writeLine("<4   ^2  8v   6>", 1);
  }
}
