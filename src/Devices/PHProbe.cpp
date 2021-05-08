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
  Serial1.print("*OK,0\r");  // Turn off the returning of OK after command to EZO pH
  Serial1.print("C,1\r");    // Reset pH stamp to continuous measurement: once per second
}

void PHProbe::clearCalibration() {
  Serial1.print("Cal,clear\r");  // send that string to the Atlas Scientific product
}

void PHProbe::sendSlopeRequest() {
  Serial1.print(F("Slope,?\r"));  // Sending request for Calibration Slope
  slopeResponse = "       Slope requested!";
}

String PHProbe::getSlope() {
  // for example "?Slope,99.7,100.3, -0.89"
  if (slopeResponse.length() < 10) {
    return String("");
  }
  String slope = slopeResponse.substring(7);
  // output to log
  serial("Calibration Slope: %s", slope.c_str());
  return slope;
}

/**
 * interrupt handler for data arriving from probe
 */
void PHProbe::serialEvent1() {
  while (Serial1.available() > 0) {               // if we see that the Atlas Scientific product has sent a character
    String string = Serial1.readStringUntil(13);  // read the string until we see a <CR>
    string.remove(string.length() - 1);
    serial("Serial1 = %s", string.c_str());
    if (string.length() > 0) {
      if (isdigit(string[0])) {  // if the first character in the string is a digit
        // convert the string to a floating point number so it can be evaluated by the Arduino
        value = string.toFloat();
      } else if (string[0] == '?') {  // answer to a previous query
        if (string.length() > 7 && string.substring(0, 7) == "?Slope,") {
          // for example "?Slope,99.7,100.3, -0.89\r"
          slopeResponse = string;
        }
      }
    }
  }
}

// "pH decreases with increase in temperature. But this does not mean that
//  water becomes more acidic at higher temperatures."
// https://www.westlab.com/blog/2017/11/15/how-does-temperature-affect-ph
void PHProbe::setTemperatureCompensation(double temperature) {
  const String PARTIAL_COMMAND = "T,";
  String fullCommand;
  if (temperature > 0 && temperature < 100) {
    fullCommand = PARTIAL_COMMAND + String(temperature, 2) + "\r";
  } else {
    fullCommand = PARTIAL_COMMAND + "20\r";
  }
  serial("PHProbe::setTemperatureCompensation) - ", (const char *)fullCommand.c_str());
  Serial1.print(fullCommand);  // send that string to the Atlas Scientific product
}

void PHProbe::setHighpointCalibration(double highpoint) {
  String fullCommand;
  fullCommand = "Cal,High," + String(highpoint, 3) + "\r";
  Serial1.print(fullCommand);  // send that string to the Atlas Scientific product
}

void PHProbe::setLowpointCalibration(double lowpoint) {
  String fullCommand;
  fullCommand = "Cal,low," + String(lowpoint, 3) + "\r";
  Serial1.print(fullCommand);  // send that string to the Atlas Scientific product
}

void PHProbe::setMidpointCalibration(double midpoint) {
  String fullCommand;
  fullCommand = "Cal,mid," + String(midpoint, 3) + "\r";
  Serial1.print(fullCommand);  // send that string to the Atlas Scientific product
}
