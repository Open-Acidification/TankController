/**
 *  This class is for the Serial Commands used in Tank Controller.
 *  Serial here mostly deals with writing or printing data over through the
 * serial port. Serial.print and Serial.write can be called in different ways to
 * print data. Also print_PID, print_DataTime, print_mac are used to print the
 * PID information, current date information, and the mac address.
 */
#pragma once

#include <Arduino.h>

class Serial_TC {
public:
  //  class methods
  static Serial_TC *instance();

  //  instance methods
  void print(char aChar) const;
  void print(uint16_t anInt) const;
  void print(double aDouble, int scale = 2) const;
  void print(const String aString, bool newLine = true) const;
  void print(const String aString, char aChar) const;
  void print(const String aString, int anInt) const;
  void print(const String aString, int anInt, int format) const;
  void print(const char *aString, const char *aString2) const;
  void print(const String aString, const String aString2) const;
  void println() const {
    Serial.println();
  }
  void print_mac(const byte mac[]) const;
  void print_two_digits(int value) const;
  void write(byte aByte) const;
  void write(const char arr[], int anInt) const;

private:
  //  class variable
  static Serial_TC *_instance;

  //  instance methods
  Serial_TC();
};
