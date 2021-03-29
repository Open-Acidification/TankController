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
  Serial1.print(F("*OK,0\r"));  // Turn off the returning of OK after command to EZO pH
  Serial1.print(F("C,1\r"));    // Reset pH stamp to continuous measurement: once per second
}

/**
 * data arriving from probe
 */
void PHProbe::serialEvent1() {
  String string = Serial1.readStringUntil(13);  // read the string until we see a <CR>
  Serial_TC *serial = Serial_TC::instance();
  if (string.length() > 0 && isdigit(string[0])) {  // if the first character in the string is a digit
    value = string.toFloat();  // convert the string to a floating point number so it can be evaluated by the Arduino
    serial->print(F("pH = "), false);
    serial->print(value, 3);
    serial->println();
  }
}

// "pH decreases with increase in temperature. But this does not mean that
//  water becomes more acidic at higher temperatures."
// https://www.westlab.com/blog/2017/11/15/how-does-temperature-affect-ph
void PHProbe::setTemperatureCompensation(double temperature) {
  const String PARTIAL_COMMAND = "T,";
  String fullCommand;
  if (temperature > 0 && temperature < 100) {
    fullCommand = PARTIAL_COMMAND + String(temperature, 2);
  } else {
    fullCommand = PARTIAL_COMMAND + "20";
  }
  Serial.println(fullCommand);
  Serial1.print(fullCommand);  // send that string to the Atlas Scientific product
  Serial1.print('\r');         // add a <CR> to the end of the string
}
