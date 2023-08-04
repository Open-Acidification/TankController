#include "MainMenu.h"

#include "BadPHCalibration.h"
#include "CalibrationManagement.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "Devices/PHProbe.h"
#include "Devices/ThermalControl.h"
#include "Devices/ThermalProbe_TC.h"
#include "EnablePID.h"
#include "PHCalibrationPrompt.h"
#include "SeeDeviceAddress.h"
#include "SeeDeviceUptime.h"
#include "SeeFreeMemory.h"
#include "SeeGoogleMins.h"
#include "SeeLogFile.h"
#include "SeePHCalibration.h"
#include "SeePIDConstants.h"
#include "SeeTankID.h"
#include "SeeThermalCorrection.h"
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
#include "ResetThermalCalibration.h"
#include "SetThermalSineWave.h"
#include "SetThermalTarget.h"
#include "SetTime.h"
#include "ThermalCalibration.h"

MainMenu::MainMenu() : UIState() {
  viewMenus[VIEW_ADDRESS] = F("View IP and MAC");
  viewMenus[VIEW_FREE_MEMORY] = F("View free memory");
  viewMenus[VIEW_GOOGLE_MINS] = F("View Google mins");
  viewMenus[VIEW_LOG_FILE] = F("View log file");
  viewMenus[VIEW_PH_SLOPE] = F("View pH slope");
  viewMenus[VIEW_PID] = F("View PID");
  viewMenus[VIEW_TANK_ID] = F("View tank ID");
  viewMenus[VIEW_THERMAL_CORRECTION] = F("View temp cal");
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
      this->setNextState(new SetPHSetPoint());
      break;
    case 'B':  // Set Temperature set_point
      this->setNextState(new SetThermalTarget());
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
      this->setNextState(new SeeDeviceAddress());
      break;
    case VIEW_FREE_MEMORY:
      this->setNextState(new SeeFreeMemory());
      break;
    case VIEW_GOOGLE_MINS:
      this->setNextState(new SeeGoogleMins());
      break;
    case VIEW_LOG_FILE:
      this->setNextState(new SeeLogFile());
      break;
    case VIEW_PH_SLOPE:
      this->setNextState(new SeePHCalibration());
      break;
    case VIEW_PID:
      this->setNextState(new SeePIDConstants());
      break;
    case VIEW_TANK_ID:
      this->setNextState(new SeeTankID());
      break;
    case VIEW_THERMAL_CORRECTION:
      this->setNextState(new SeeThermalCorrection());
      break;
    case VIEW_TIME:
      this->setNextState(new SeeDeviceUptime());
      break;
    case VIEW_VERSION:
      this->setNextState(new SeeVersion());
      break;
    default:
      break;
  }
}

void MainMenu::selectSet() {
  switch (level2) {
    case SET_CALIBRATION:
      this->setNextState(new PHCalibrationPrompt());
      break;
    case SET_PH_CALIBRATION_CLEAR:
      this->setNextState(new SetPHCalibClear());
      break;
    case SET_TEMP_CALIBRATION_CLEAR:
      this->setNextState(new ResetThermalCalibration());
      break;
    case SET_CHILL_OR_HEAT:
      this->setNextState(new SetChillOrHeat());
      break;
    case SET_GOOGLE_MINS:
      this->setNextState(new SetGoogleSheetInterval());
      break;
    case SET_KD:
      this->setNextState(new SetKD());
      break;
    case SET_KI:
      this->setNextState(new SetKI());
      break;
    case SET_KP:
      this->setNextState(new SetKP());
      break;
    case SET_PH:
      this->setNextState(new SetPHSetPoint());
      break;
    case SET_PH_WITH_SINE:
      this->setNextState(new SetPHWithSine());
      break;
    case SET_TEMP_WITH_SINE:
      this->setNextState(new SetThermalSineWave());
      break;
    case SET_PID_ON_OFF:
      this->setNextState(new EnablePID());
      break;
    case SET_TANK_ID:
      this->setNextState(new SetTankID());
      break;
    case SET_TEMP_CALIBRATION:
      this->setNextState(new ThermalCalibration());
      break;
    case SET_TEMPERATURE:
      this->setNextState(new SetThermalTarget());
      break;
    case SET_TIME:
      this->setNextState(new SetTime());
      break;
    default:
      break;
  }
}

// pH=7.325 B 7.125
// T=12.25 H 12.75
void MainMenu::idle() {
  if (PHProbe::instance()->shouldWarnAboutCalibration()) {
    this->setNextState(new BadPHCalibration());
    return;
  }
  char buffer[6];
  char output[20];
  bool pHBlink = PHProbe::instance()->slopeIsBad() && ((millis() + 1000) / 1000 % 2);
  output[0] = pHBlink ? ' ' : 'p';
  output[1] = pHBlink ? ' ' : 'H';
  output[2] = millis() / 1000 % 2 ? '=' : ' ';
  float pH = PHProbe::instance()->getPh();
  if (pH < 10.0) {
    floattostrf(pH, 5, 3, buffer, sizeof(buffer));
  } else {
    floattostrf(pH, 5, 2, buffer, sizeof(buffer));
  }
  memcpy(output + 3, buffer, sizeof(buffer));
  output[8] = ' ';
  output[9] = PHControl::instance()->isOn() ? 'B' : ' ';
  output[10] = ' ';
  pH = PHControl::instance()->getCurrentTargetPh();
  if (pH < 10.0) {
    floattostrf(pH, 5, 3, buffer, sizeof(buffer));
  } else {
    floattostrf(pH, 5, 2, buffer, sizeof(buffer));
  }
  memcpy(output + 11, buffer, sizeof(buffer));
  LiquidCrystal_TC::instance()->writeLine(output, 0);
  ThermalControl *thermalControl = ThermalControl::instance();
  float temp = ThermalProbe_TC::instance()->getRunningAverage();
  char status = thermalControl->isHeater() ? 'h' : 'c';
  if (thermalControl->isOn()) {
    status = toupper(status);
  }
  output[0] = 'T';
  output[1] = millis() / 1000 % 2 ? '=' : ' ';
  floattostrf(temp, 5, 2, buffer, sizeof(buffer));
  memcpy(output + 2, buffer, sizeof(buffer));
  output[7] = ' ';
  output[8] = status;
  output[9] = ' ';
  floattostrf(thermalControl->getCurrentThermalTarget(), 5, 2, buffer, sizeof(buffer));
  memcpy(output + 10, buffer, sizeof(buffer));
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
