#pragma once

#include <Arduino.h>

#define SS 4
#include <SdFat.h>

class SD_TC {
public:
  // class methods
  static SD_TC* instance();

  // instance methods
  void appendData(const char* header, const char* line);
  void appendToLog(const char* line);
  bool exists(const char* path);
  bool format();
  bool mkdir(const char* path);
  File open(const char* path, oflag_t oflag = 0x00);
  void todaysDataFileName(char* path, int size);
  void printRootDirectory();
  void writePhPoints(const char* line);
  const char * readTextFileLine(const char * path, char* bytes, uint32_t seekTo = 0);
  // uint32_t SD_TC::position();
private:
  // class variables
  static SD_TC* _instance;

  // instance variables
  const uint8_t SD_SELECT_PIN = SS;
  bool hasHadError = false;
  SdFat sd;

  // instance methods
  SD_TC();
  void appendDataToPath(const char* data, const char* path);
};
