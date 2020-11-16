/**
 *  This class is for the Serial Commands used in Tank Controller.
 *  Serial here mostly deals with writing or printing data over through the serial port.
 *  Serial.print and Serial.write can be called in different ways to print data.
 *  Also print_PID, print_DataTime, print_mac are used to print the PID information,
 *  current date information, and the mac address.
 */
#include <Arduino.h>
#include <RTClib.h>

class Serial_TC {
private:
  void print_two_digits(int value) {
    if (value < 10) {
      Serial.print('0');
    }
    Serial.print(value, DEC);
  }

public:
  Serial_TC() {
    Serial.begin(9600);
  }

  /**
   * Print the PID.
   */
  void print_PID(double Kp, double Ki, double Kd, double output) {
    Serial.print(F("Kp:"));
    Serial.print(Kp);
    Serial.print(F(" Ki:"));
    Serial.print(Ki);
    Serial.print(F(" Kd:"));
    Serial.println(Kd);
    Serial.print(F("PID output (s): "));
    Serial.println(output / 1000, 1);
  }

  void print_DateTime(DateTime dateTime) {
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

  void print_mac(byte mac[]) {
    Serial.print(F("MAC Address: "));
    Serial.print(mac[0]);
    for (int i = 1; i < 6; ++i) {
      Serial.print(':');
      Serial.print(mac[i]);
    }
    Serial.println();
  }

  void print(String aString, String aString2) {
    Serial.println(aString);
    Serial.println(aString2);
  }

  void print(String aString, int anInt) {
    Serial.print(aString);
    Serial.println(anInt);
  }

  void print(String aString, int anInt, int format) {
    Serial.print(aString);
    Serial.println(anInt, format);
  }

  void print(String aString) {
    Serial.println(aString);
  }

  void write(byte aByte) {
    Serial.write(aByte);
  }

  void write(char arr[], int anInt) {
    Serial.write(arr, anInt);
  }
};
