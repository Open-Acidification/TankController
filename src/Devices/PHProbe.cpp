#include "Devices/PHProbe.h"

#include <avr/wdt.h>
#include <stdlib.h>

#include "Devices/Serial_TC.h"
#include "TC_util.h"

#if defined(ARDUINO_CI_COMPILATION_MOCKS)
#include <Arduino.h>

#include "TankController.h"
#endif

//  class instance variables
/**
 * static variable for singleton
 */
PHProbe *PHProbe::_instance = nullptr;

//  class methods
/**
 * static member function to return singleton
 */
PHProbe *PHProbe::instance() {
  if (!_instance) {
    _instance = new PHProbe();
  }
  return _instance;
}

//  instance methods
/**
 * constructor (private so clients use the singleton)
 */
PHProbe::PHProbe() {
  Serial1.begin(9600);
  // wait for Serial Monitor to connect. Needed for native USB port boards only:
  while (!Serial1)
    ;
  Serial1.print(F("*OK,0\r"));  // Turn off the returning of OK after command to EZO pH
  Serial1.print(F("C,1\r"));    // Reset pH stamp to continuous measurement: once per second
}

void PHProbe::clearCalibration() {
  Serial1.print(F("Cal,clear\r"));  // send that string to the Atlas Scientific product
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  this->calibrationPoints = 0;
#endif
}

void PHProbe::sendCalibrationRequest() {
  // Sending request for calibration status
  Serial1.print(F("Cal,?\r"));
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  GodmodeState *state = GODMODE();
  char buffer[10];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("?Cal,%i\r"), this->calibrationPoints);
  state->serialPort[1].dataIn = buffer;        // the queue of data waiting to be read
  TankController::instance()->serialEvent1();  // fake interrupt
#endif
  strscpy_P(calibrationResponse, F("Requesting..."), sizeof(calibrationResponse));
}

void PHProbe::getCalibration(char *buffer, int size) {
  // for example "2" or "Requesting..."
  strscpy(buffer, calibrationResponse, size);
}

void PHProbe::sendSlopeRequest() {
  // Sending request for Calibration Slope
  Serial1.print(F("SLOPE,?\r"));
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  GodmodeState *state = GODMODE();
  char buffer[10];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("?SLOPE,99.7,100.3,-0.89\r"));
  state->serialPort[1].dataIn = buffer;        // the queue of data waiting to be read
  TankController::instance()->serialEvent1();  // fake interrupt
#endif
  strscpy_P(slopeResponse, F("Requesting..."), sizeof(slopeResponse));
}

void PHProbe::getSlope(char *buffer, int size) {
  // for example "99.7,100.3, -0.89" or "Requesting..."
  strscpy(buffer, slopeResponse, size);
}

/**
 * interrupt handler for data arriving from probe
 */
void PHProbe::serialEvent1() {
  // if we see that the Atlas Scientific product has sent a character
  while (Serial1.available() > 0) {
    String string = Serial1.readStringUntil('\r');  // read the string until we see a <CR>
    if (string.length() > 0 && string[string.length() - 1] == '\r') {
      // We should not see the CR (https://github.com/Arduino-CI/arduino_ci/pull/302)
      string.remove(string.length() - 1);
    }
    if (string.length() > 0) {
      if (isdigit(string[0])) {  // if the first character in the string is a digit
        // convert the string to a floating point number so it can be evaluated by the Arduino
        value = string.toFloat();
      } else if (string[0] == '?') {  // answer to a previous query
        serial(F("PHProbe serialEvent1: \"%s\""), string.c_str());
        if (string.length() > 7 && memcmp_P(string.c_str(), F("?SLOPE,"), 7) == 0) {
          // for example "?SLOPE,16.1,100.0"
          strscpy(slopeResponse, string.c_str() + 7, sizeof(slopeResponse));  // Flawfinder: ignore
        } else if (string.length() > 5 && memcmp_P(string.c_str(), F("?Cal,"), 5) == 0) {
          // for example "?Cal,2"
          snprintf_P(calibrationResponse, sizeof(calibrationResponse), PSTR("%s point"),
                     string.c_str() + 5);  // Flawfinder: ignore
        }
      }
    }
  }
}

// "pH decreases with increase in temperature. But this does not mean that
//  water becomes more acidic at higher temperatures."
// https://www.westlab.com/blog/2017/11/15/how-does-temperature-affect-ph
void PHProbe::setTemperatureCompensation(float temperature) {
  char buffer[10];
  if (temperature > 0 && temperature < 100) {
    snprintf_P(buffer, sizeof(buffer), (PGM_P)F("T,%i.%02i\r"), (int)temperature, (int)(temperature * 100 + 0.5) % 100);
  } else {
    snprintf_P(buffer, sizeof(buffer), (PGM_P)F("T,20\r"));
  }
  serial(F("PHProbe::setTemperatureCompensation() - %s"), buffer);
  Serial1.print(buffer);  // send that string to the Atlas Scientific product
}

void PHProbe::setHighpointCalibration(float highpoint) {
  char buffer[17];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("Cal,High,%i.%03i\r"), (int)highpoint,
             (int)(highpoint * 1000 + 0.5) % 1000);
  Serial1.print(buffer);  // send that string to the Atlas Scientific product
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  this->calibrationPoints += 1;
#endif
  serial(F("PHProbe::setHighpointCalibration(%i.%03i)"), (int)highpoint, (int)(highpoint * 1000) % 1000);
}

void PHProbe::setLowpointCalibration(float lowpoint) {
  char buffer[16];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("Cal,low,%i.%03i\r"), (int)lowpoint, (int)(lowpoint * 1000 + 0.5) % 1000);
  Serial1.print(buffer);  // send that string to the Atlas Scientific product
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  this->calibrationPoints += 1;
#endif
  serial(F("PHProbe::setLowpointCalibration(%i.%03i)"), (int)lowpoint, (int)(lowpoint * 1000) % 1000);
}

void PHProbe::setMidpointCalibration(float midpoint) {
  char buffer[16];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("Cal,mid,%i.%03i\r"), (int)midpoint, (int)(midpoint * 1000 + 0.5) % 1000);
  Serial1.print(buffer);  // send that string to the Atlas Scientific product
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  this->calibrationPoints = 1;
#endif
  serial(F("PHProbe::setMidpointCalibration(%i.%03i)"), (int)midpoint, (int)(midpoint * 1000) % 1000);
}

#if defined(ARDUINO_CI_COMPILATION_MOCKS)
void PHProbe::setCalibrationPoints(int newValue) {
  this->calibrationPoints = newValue;
  GodmodeState *state = GODMODE();
  char buffer[10];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("?Cal,%i\r"), this->calibrationPoints);
  state->serialPort[1].dataIn = buffer;        // the queue of data waiting to be read
  TankController::instance()->serialEvent1();  // fake interrupt
}

void PHProbe::setPh(float newValue) {
  GodmodeState *state = GODMODE();
  char buffer[10];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("%i.%03i\r"), (int)newValue, (int)(newValue * 1000 + 0.5) % 1000);
  state->serialPort[1].dataIn = buffer;        // the queue of data waiting to be read
  TankController::instance()->serialEvent1();  // fake interrupt to update the current pH reading
  TankController::instance()->loop();          // update the controls based on the current readings
}
#endif
