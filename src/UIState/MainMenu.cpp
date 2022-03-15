#include "MainMenu.h"

#include "CalibrationManagement.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "Devices/PHProbe.h"
#include "Devices/TempProbe_TC.h"
#include "Devices/TemperatureControl.h"
#include "EnablePID.h"
#include "PHCalibrationMid.h"
#include "SeeDeviceAddress.h"
#include "SeeDeviceUptime.h"
#include "SeeFreeMemory.h"
#include "SeeGoogleMins.h"
#include "SeeLogFile.h"
#include "SeePHSlope.h"
#include "SeePIDConstants.h"
#include "SeeTankID.h"
#include "SeeVersion.h"
#include "SetChillOrHeat.h"
#include "SetGoogleSheetInterval.h"
#include "SetKD.h"
#include "SetKI.h"
#include "SetKP.h"
#include "SetPHCalibClear.h"
#include "SetPHSetPoint.h"
#include "SetPHWithSine.h"
#include "SetTankID.h"
#include "SetTempCalibClear.h"
#include "SetTempSetPoint.h"
#include "SetTempWithSine.h"
#include "SetTime.h"
#include "TemperatureCalibration.h"

MainMenu::MainMenu(TankController *tc) : UIState(tc) {
  viewMenus[VIEW_ADDRESS] = F("View IP and MAC");
  viewMenus[VIEW_FREE_MEMORY] = F("View free memory");
  viewMenus[VIEW_GOOGLE_MINS] = F("View Google mins");
  viewMenus[VIEW_LOG_FILE] = F("View log file");
  viewMenus[VIEW_PH_SLOPE] = F("View pH slope");
  viewMenus[VIEW_PID] = F("View PID");
  viewMenus[VIEW_TANK_ID] = F("View tank ID");
  viewMenus[VIEW_TIME] = F("View time");
  viewMenus[VIEW_VERSION] = F("View version");

  setMenus[SET_CALIBRATION] = F("pH calibration");
  setMenus[SET_PH_CALIBRATION_CLEAR] = F("Clear pH calibra");
  setMenus[SET_TEMP_CALIBRATION_CLEAR] = F("Clear Temp calib");
  setMenus[SET_CHILL_OR_HEAT] = F("Set chill/heat");
  setMenus[SET_GOOGLE_MINS] = F("Set Google mins");
  setMenus[SET_KD] = F("Set KD");
  setMenus[SET_KI] = F("Set KI");
  setMenus[SET_KP] = F("Set KP");
  setMenus[SET_PH] = F("Set pH target");
  setMenus[SET_PH_WITH_SINE] = F("Set pH w sine");
  setMenus[SET_TEMP_WITH_SINE] = F("Set Temp w sine");
  setMenus[SET_PID_ON_OFF] = F("PID on/off");
  setMenus[SET_TANK_ID] = F("Set Tank ID");
  setMenus[SET_TEMP_CALIBRATION] = F("Temp calibration");
  setMenus[SET_TEMPERATURE] = F("Set temperature");
  setMenus[SET_TIME] = F("Set date/time");
}

/**
 * Branch to other states to handle various menu options
 */
void MainMenu::handleKey(char key) {
  switch (key) {
    case 'A':  // Set pH set_point
      this->setNextState(static_cast<UIState *>(new SetPHSetPoint(tc)));
      break;
    case 'B':  // Set Temperature set_point
      this->setNextState(static_cast<UIState *>(new SetTempSetPoint(tc)));
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
    case VIEW_ADDRESS:
      this->setNextState(static_cast<UIState *>(new SeeDeviceAddress(tc)));
      break;
    case VIEW_FREE_MEMORY:
      this->setNextState(static_cast<UIState *>(new SeeFreeMemory(tc)));
      break;
    case VIEW_GOOGLE_MINS:
      this->setNextState(static_cast<UIState *>(new SeeGoogleMins(tc)));
      break;
    case VIEW_LOG_FILE:
      this->setNextState(static_cast<UIState *>(new SeeLogFile(tc)));
      break;
    case VIEW_PH_SLOPE:
      this->setNextState(static_cast<UIState *>(new SeePHSlope(tc)));
      break;
    case VIEW_PID:
      this->setNextState(static_cast<UIState *>(new SeePIDConstants(tc)));
      break;
    case VIEW_TANK_ID:
      this->setNextState(static_cast<UIState *>(new SeeTankID(tc)));
      break;
    case VIEW_TIME:
      this->setNextState(static_cast<UIState *>(new SeeDeviceUptime(tc)));
      break;
    case VIEW_VERSION:
      this->setNextState(static_cast<UIState *>(new SeeVersion(tc)));
      break;
    default:
      break;
  }
}

void MainMenu::selectSet() {
  switch (level2) {
    case SET_CALIBRATION:
      this->setNextState(static_cast<UIState *>(new PHCalibrationMid(tc)));
      break;
    case SET_PH_CALIBRATION_CLEAR:
      this->setNextState(static_cast<UIState *>(new SetPHCalibClear(tc)));
      break;
    case SET_TEMP_CALIBRATION_CLEAR:
      this->setNextState(static_cast<UIState *>(new SetTempCalibClear(tc)));
      break;
    case SET_CHILL_OR_HEAT:
      this->setNextState(static_cast<UIState *>(new SetChillOrHeat(tc)));
      break;
    case SET_GOOGLE_MINS:
      this->setNextState(static_cast<UIState *>(new SetGoogleSheetInterval(tc)));
      break;
    case SET_KD:
      this->setNextState(static_cast<UIState *>(new SetKD(tc)));
      break;
    case SET_KI:
      this->setNextState(static_cast<UIState *>(new SetKI(tc)));
      break;
    case SET_KP:
      this->setNextState(static_cast<UIState *>(new SetKP(tc)));
      break;
    case SET_PH:
      this->setNextState(static_cast<UIState *>(new SetPHSetPoint(tc)));
      break;
    case SET_PH_WITH_SINE:
      this->setNextState(static_cast<UIState *>(new SetPHWithSine(tc)));
      break;
    case SET_TEMP_WITH_SINE:
      this->setNextState(static_cast<UIState *>(new SetTempWithSine(tc)));
      break;
    case SET_PID_ON_OFF:
      this->setNextState(static_cast<UIState *>(new EnablePID(tc)));
      break;
    case SET_TANK_ID:
      this->setNextState(static_cast<UIState *>(new SetTankID(tc)));
      break;
    case SET_TEMP_CALIBRATION:
      this->setNextState(static_cast<UIState *>(new TemperatureCalibration(tc)));
      break;
    case SET_TEMPERATURE:
      this->setNextState(static_cast<UIState *>(new SetTempSetPoint(tc)));
      break;
    case SET_TIME:
      this->setNextState(static_cast<UIState *>(new SetTime(tc)));
      break;
    default:
      break;
  }
}

// pH=7.325 B 7.125
// T=12.25 H 12.75
void MainMenu::idle() {
  PHControl *phControl = PHControl::instance();
  char output[20];
  output[0] = 'p';
  output[1] = 'H';
  output[2] = millis() / 1000 % 2 ? '=' : ' ';
  dtostrf(PHProbe::instance()->getPh(), 5, 3, output + 3);
  output[8] = ' ';
  output[9] = phControl->isOn() ? 'B' : ' ';
  output[10] = ' ';
  dtostrf(PHControl::instance()->getTargetPh(), 5, 3, output + 11);
  LiquidCrystal_TC::instance()->writeLine(output, 0);
  TemperatureControl *tempControl = TemperatureControl::instance();
  TempProbe_TC *tempProbe = TempProbe_TC::instance();
  float temp = tempProbe->getRunningAverage();
  if (temp < 0.0) {
    temp = 0.0;
  } else if (99.99 < temp) {
    temp = 99.99;
  }
  char status = tempControl->isHeater() ? 'h' : 'c';
  if (tempControl->isOn()) {
    status = toupper(status);
  }
  output[0] = 'T';
  output[1] = millis() / 1000 % 2 ? '=' : ' ';
  dtostrf(temp, 5, 2, output + 2);
  output[7] = ' ';
  output[8] = status;
  output[9] = ' ';
  dtostrf(tempControl->getTargetTemperature(), 5, 2, output + 10);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
}

void MainMenu::loop() {
  if (level1 == 0) {
    idle();
  } else {
    if (level1 == 1) {
      if (level2 == -1) {
        LiquidCrystal_TC::instance()->writeLine(F("View settings"), 0);
      } else {
        LiquidCrystal_TC::instance()->writeLine(viewMenus[level2], 0);
      }
    } else {
      if (level2 == -1) {
        LiquidCrystal_TC::instance()->writeLine(F("Change settings"), 0);
      } else {
        LiquidCrystal_TC::instance()->writeLine(setMenus[level2], 0);
      }
    }
    LiquidCrystal_TC::instance()->writeLine(F("<4   ^2  8v   6>"), 1);
  }
}
