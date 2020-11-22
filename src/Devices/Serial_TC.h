/**
 *  This class is for the Serial Commands used in Tank Controller.
 *  Serial here mostly deals with writing or printing data over through the serial port.
 *  Serial.print and Serial.write can be called in different ways to print data.
 *  Also print_PID, print_DataTime, print_mac are used to print the PID information,
 *  current date information, and the mac address.
 */
#pragma once

#include <Arduino.h>
#include <RTClib.h>

class Serial_TC {
public:
  //  class methods
  static Serial_TC* instance();

  //  instance methods
  void print(String aString);
  void print(String aString, char aChar);
  void print(String aString, int anInt);
  void print(String aString, int anInt, int format);
  void print(String aString, String aString2);
  void print_DateTime(DateTime dateTime);
  void print_mac(byte mac[]);
  void print_PID(double Kp, double Ki, double Kd, double output);
  void write(byte aByte);
  void write(char arr[], int anInt);

private:
  //  class variable
  static Serial_TC* _instance;

  //  instance methods
  Serial_TC();
  void print_two_digits(int value);
};
