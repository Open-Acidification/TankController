#include "Devices/PHProbe.h"

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
  Serial1.print("*OK,0\r");  // Turn off the returning of OK after command to EZO pH
  Serial1.print("C,1\r");    // Reset pH stamp to continuous measurement: once per second
}

void PHProbe::clearCalibration() {
  Serial1.print("Cal,clear\r");  // send that string to the Atlas Scientific product
}

void PHProbe::sendSlopeRequest() {
  Serial1.print("SLOPE,?\r");  // Sending request for Calibration Slope
  slopeResponse = "       Slope requested!";
}

String PHProbe::getSlope() {
  // for example "?SLOPE,99.7,100.3, -0.89"
  if (slopeResponse.length() < 10) {
    return String("");
  }
  String slope = slopeResponse.substring(7);
  return slope;
}

/**
 * interrupt handler for data arriving from probe
 */
void PHProbe::serialEvent1() {
  while (Serial1.available() > 0) {               // if we see that the Atlas Scientific product has sent a character
    String string = Serial1.readStringUntil(13);  // read the string until we see a <CR>
    string.remove(string.length() - 1);
    if (string.length() > 0) {
      if (isdigit(string[0])) {  // if the first character in the string is a digit
        // convert the string to a floating point number so it can be evaluated by the Arduino
        value = string.toFloat();
      } else if (string[0] == '?') {  // answer to a previous query
        serial("PHProbe serialEvent1: \"%s\"", string.c_str());
        if (string.length() > 7 && string.substring(0, 7) == "?SLOPE,") {
          // for example "?SLOPE,16.1,100.0"
          slopeResponse = string;
        }
      }
    }
  }
}

// "pH decreases with increase in temperature. But this does not mean that
//  water becomes more acidic at higher temperatures."
// https://www.westlab.com/blog/2017/11/15/how-does-temperature-affect-ph
void PHProbe::setTemperatureCompensation(float temperature) {
  const String PARTIAL_COMMAND = "T,";
  String fullCommand;
  if (temperature > 0 && temperature < 100) {
    fullCommand = PARTIAL_COMMAND + String(temperature, 2) + "\r";
  } else {
    fullCommand = PARTIAL_COMMAND + "20\r";
  }
  serial("PHProbe::setTemperatureCompensation) - %s", fullCommand.c_str());
  Serial1.print(fullCommand);  // send that string to the Atlas Scientific product
}

void PHProbe::setHighpointCalibration(float highpoint) {
  String fullCommand;
  fullCommand = "Cal,High," + String(highpoint, 3) + "\r";
  Serial1.print(fullCommand);  // send that string to the Atlas Scientific product
}

void PHProbe::setLowpointCalibration(float lowpoint) {
  String fullCommand;
  fullCommand = "Cal,low," + String(lowpoint, 3) + "\r";
  Serial1.print(fullCommand);  // send that string to the Atlas Scientific product
}

void PHProbe::setMidpointCalibration(float midpoint) {
  String fullCommand;
  fullCommand = "Cal,mid," + String(midpoint, 3) + "\r";
  Serial1.print(fullCommand);  // send that string to the Atlas Scientific product
}
