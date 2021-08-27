#include "Devices/PHProbe.h"

#include <avr/wdt.h>

#include "Devices/Serial_TC.h"

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
}

void PHProbe::sendSlopeRequest() {
  // Sending request for Calibration Slope
  Serial1.print(F("SLOPE,?\r"));
  strncpy_P(slopeResponse, (PGM_P)F("       Slope requested!"), sizeof(slopeResponse));  // Flawfinder: ignore
}

void PHProbe::getSlope(char *buffer, int size) {
  // for example "?SLOPE,99.7,100.3, -0.89"
  if (strlen(slopeResponse) > 10) {            // Flawfinder: ignore
    strncpy(buffer, slopeResponse + 7, size);  // Flawfinder: ignore
  } else {
    buffer[0] = '\0';
  }
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
        // we have seen situations where the CO2 bubbler stays on and drives the pH down
        if (value && value < 7.0) {  // hang so as to trigger the watchdog timer reset
                                     // treat 0 as valid since probe might not be connected
          wdt_disable();
          wdt_enable(WDTO_120MS);  // allow enough time to print message
          serial(F("pH value dropped to %5.3f so trigger a reset!"), value);
          while (true) {
          }
        }
      } else if (string[0] == '?') {  // answer to a previous query
        serial(F("PHProbe serialEvent1: \"%s\""), string.c_str());
        if (string.length() > 7 && string.substring(0, 7) == "?SLOPE,") {
          // for example "?SLOPE,16.1,100.0"
          strncpy(slopeResponse, string.c_str(), sizeof(slopeResponse));  // Flawfinder: ignore
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
    snprintf_P(buffer, sizeof(buffer), (PGM_P)F("T,%.2f\r"), temperature);
  } else {
    snprintf_P(buffer, sizeof(buffer), (PGM_P)F("T,20\r"));
  }
  serial(F("PHProbe::setTemperatureCompensation() - %s"), buffer);
  Serial1.print(buffer);  // send that string to the Atlas Scientific product
}

void PHProbe::setHighpointCalibration(float highpoint) {
  char buffer[17];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("Cal,High,%.3f\r"), highpoint);
  Serial1.print(buffer);  // send that string to the Atlas Scientific product
}

void PHProbe::setLowpointCalibration(float lowpoint) {
  char buffer[16];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("Cal,low,%.3f\r"), lowpoint);
  Serial1.print(buffer);  // send that string to the Atlas Scientific product
}

void PHProbe::setMidpointCalibration(float midpoint) {
  char buffer[16];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("Cal,mid,%.3f\r"), midpoint);
  Serial1.print(buffer);  // send that string to the Atlas Scientific product
}
