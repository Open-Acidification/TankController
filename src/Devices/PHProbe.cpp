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
void PHProbe::probeData(String string) {
  Serial_TC *serial = Serial_TC::instance();
  if (string.size() > 0 && isdigit(string[0])) {  // if the first character in the string is a digit
    value = string.toFloat();  // convert the string to a floating point number so it can be evaluated by the Arduino
    serial->print(F("pH = "), false);
    serial->print(value, 3);
    serial->println();
  }
}
