// /**
//  * SetRampTime.cpp
//  */

// #include "SetRampTime.h"

// #include "Devices/DateTime_TC.h"
// #include "Devices/EEPROM_TC.h"
// #include "Devices/LiquidCrystal_TC.h"
// #include "Devices/Serial_TC.h"
// #include "Devices/PHControl.h"

// SetRampTime::SetRampTime(TankController* tc) : NumCollectorState(tc) {
//   prompts[0] = F("Set Ph:");
//   prompts[1] = F("Set ramp hours:");
// }

// float SetRampTime::getCurrentValue() {
//   if (subState == 0) {
//     // initialize current values with current date time
//     int time = PHControl::instance()->getRampTime();

//     int hour = time / 3600;
//     time = time % 3600;
//     int min = time / 60;
//     time = time % 60;
//     int sec = time;

//     current[0] = hour;
//     current[1] = min;
//     current[2] = sec;
//   }
//   return current[subState];
// }

// void SetRampTime::setValue(float value) {
//   values[subState++] = value;
//   if (subState < NUM_VALUES) {
//     clear();
//     start();
//   } else {
//     int rampTimeInSeconds = values[0] * 3600 + values[1] * 60  + values[2];
//     PHControl::instance()->setRamp(rampTimeInSeconds);
//     // char* hourString = (char*) values[0];
//     // char* minuteString = (char*) values[1];
//     // char* secondString = (char*) values[2];
//     // char* buffer = hourString + ":" + minuteString + ":" + secondString;
//     // serial(F("Set ramp to %s"), buffer);
//     // LiquidCrystal_TC::instance()->writeLine(F("New Ramp Time:"), 0);
//     // LiquidCrystal_TC::instance()->writeLine(buffer, 1);
//     returnToMainMenu(3000);  // after 3-second delay
//   }
// }
