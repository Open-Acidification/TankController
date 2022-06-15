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
  bool remove(const char* myFileName);
  void writePhPoint(float value);
  float readPhPoint(const uint32_t seekTo = 0);
  // uint32_t SD_TC::position();
  void listRootToBuffer(void (*callWhenFull)(char*, bool));

private:
  // class variables
  static SD_TC* _instance;

  // instance variables
  const uint8_t SD_SELECT_PIN = SS;
  bool hasHadError = false;
  SdFat sd;
  File* hierarchy = nullptr;
  int hierarchySize = 0;
  bool inProgress = false;

  // instance methods
  SD_TC();
  void appendDataToPath(const char* data, const char* path);
  void listFiles(void (*callWhenFull)(char*, bool), byte tabulation = 0);
};
