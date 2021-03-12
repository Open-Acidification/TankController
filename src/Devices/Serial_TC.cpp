#include "Devices/Serial_TC.h"

//  class instance variables
/**
 * static variable for singleton
 */
Serial_TC *Serial_TC::_instance = nullptr;

//  class methods
/**
 * static member function to return singleton
 */
Serial_TC *Serial_TC::instance() {
  if (!_instance) {
    _instance = new Serial_TC();
  }
  return _instance;
}

//  instance methods
/**
 * constructor (private so clients use the singleton)
 */
Serial_TC::Serial_TC() {
  Serial.begin(9600);
}

void Serial_TC::print(char aChar) const {
  Serial.print(aChar);
}

void Serial_TC::print(uint16_t anInt) const {
  Serial.print(anInt);
}

void Serial_TC::print(double aDouble, int scale) const {
  Serial.print(aDouble, scale);
}

void Serial_TC::print(const String aString, bool newLine) const {
  if (newLine) {
    Serial.println(aString);
  } else {
    Serial.print(aString);
  }
}

void Serial_TC::print(const String aString, char aChar) const {
  Serial.print(aString);
  Serial.println(aChar);
}

void Serial_TC::print(const String aString, int anInt) const {
  Serial.print(aString);
  Serial.println(anInt);
}

void Serial_TC::print(const String aString, int anInt, int format) const {
  Serial.print(aString);
  Serial.println(anInt, format);
}

void Serial_TC::print(const char *aString, const char *aString2) const {
  Serial.print(aString);
  Serial.println(aString2);
}

void Serial_TC::print(const String aString, const String aString2) const {
  Serial.print(aString);
  Serial.println(aString2);
}

void Serial_TC::print_mac(const byte mac[]) const {
  Serial.print(F("MAC Address: "));
  Serial.print(mac[0]);
  for (int i = 1; i < 6; ++i) {
    Serial.print(':');
    Serial.print(mac[i]);
  }
  Serial.println();
}

/**
 * function to add a leading zero digit
 */
void Serial_TC::print_two_digits(int value) const {
  if (value < 10) {
    Serial.print('0');
  }
  Serial.print(value, DEC);
}

void Serial_TC::write(byte aByte) const {
  Serial.write(aByte);
}

void Serial_TC::write(const char arr[], int anInt) const {
  Serial.write(arr, anInt);
}
