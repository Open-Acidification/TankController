#include "Devices/Serial_TC.h"

//  class instance variables
/**
 * static variable for singleton
 */
Serial_TC* Serial_TC::_instance = nullptr;

//  class methods
/**
 * static member function to return singleton
 */
Serial_TC* Serial_TC::instance() {
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

void Serial_TC::print(String aString) {
  Serial.println(aString);
}

void Serial_TC::print(String aString, char aChar) {
  Serial.print(aString);
  Serial.println(aChar);
}

void Serial_TC::print(String aString, int anInt) {
  Serial.print(aString);
  Serial.println(anInt);
}

void Serial_TC::print(String aString, int anInt, int format) {
  Serial.print(aString);
  Serial.println(anInt, format);
}

void Serial_TC::print(String aString, String aString2) {
  Serial.print(aString);
  Serial.println(aString2);
}

void Serial_TC::print_DateTime(DateTime dateTime) {
  Serial.print(dateTime.year(), DEC);
  Serial.print('-');
  print_two_digits(dateTime.month());
  Serial.print('-');
  print_two_digits(dateTime.day());
  Serial.print(' ');
  print_two_digits(dateTime.hour());
  Serial.print(':');
  print_two_digits(dateTime.minute());
  Serial.print(':');
  print_two_digits(dateTime.second());
  Serial.println();
}

void Serial_TC::print_mac(byte mac[]) {
  Serial.print(F("MAC Address: "));
  Serial.print(mac[0]);
  for (int i = 1; i < 6; ++i) {
    Serial.print(':');
    Serial.print(mac[i]);
  }
  Serial.println();
}

/**
 * Print the PID constants and output
 */
void Serial_TC::print_PID(double Kp, double Ki, double Kd, double output) {
  Serial.print(F("Kp:"));
  Serial.print(Kp);
  Serial.print(F(" Ki:"));
  Serial.print(Ki);
  Serial.print(F(" Kd:"));
  Serial.println(Kd);
  Serial.print(F("PID output (s): "));
  Serial.println(output / 1000, 1);
}

/**
 * private function to add a leading zero digit
 */
void Serial_TC::print_two_digits(int value) {
  if (value < 10) {
    Serial.print('0');
  }
  Serial.print(value, DEC);
}

void Serial_TC::write(byte aByte) {
  Serial.write(aByte);
}

void Serial_TC::write(char arr[], int anInt) {
  Serial.write(arr, anInt);
}
