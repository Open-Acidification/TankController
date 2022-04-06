#include "Devices/SD_TC.h"

#include "DateTime_TC.h"
#include "Serial_TC.h"
#include "TC_util.h"

//  class variables
SD_TC* SD_TC::_instance = nullptr;

//  class methods
/**
 * accessor for singleton
 */
SD_TC* SD_TC::instance() {
  if (!_instance) {
    _instance = new SD_TC();
  }
  return _instance;
}

//  instance methods

/**
 * constructor
 */
SD_TC::SD_TC() {
  Serial.println(F("SD_TC()"));  // Serial_TC might not be ready yet
  assert(_instance == nullptr);
  if (!sd.begin(SD_SELECT_PIN)) {
    Serial.println(F("SD_TC failed to initialize!"));
  }
}

/**
 * append data to a data log file
 */
void SD_TC::appendData(const char* header, const char* line) {
  char path[30];
  todaysDataFileName(path, sizeof(path));
  if (!sd.exists(path)) {
    appendDataToPath(header, path);
    COUT(header);
  }
  appendDataToPath(line, path);
  COUT(line);
}

/**
 * append data to a path
 */
void SD_TC::appendDataToPath(const char* line, const char* path) {
  COUT(path);
  File file = sd.open(path, O_CREAT | O_WRONLY | O_APPEND);
  if (file) {
    file.write(line);
    file.write("\n", 1);
    file.close();
    COUT(file);
  } else {
    if (!hasHadError) {
      hasHadError = true;
      serial(F("Unable to open file: \"%s\""), path);
      COUT("Unable to open file: \"" << path << "\"");
      return;
    }
  }
}

/**
 * append data to a serial log file
 */
void SD_TC::appendToLog(const char* line) {
  DateTime_TC now = DateTime_TC::now();
  char path[30];
  snprintf_P(path, sizeof(path), (PGM_P)F("%4i%02i%02i.log"), now.year(), now.month(), now.day());
  appendDataToPath(line, path);
}

bool SD_TC::exists(const char* path) {
  return sd.exists(path);
}

bool SD_TC::format() {
  return sd.format();
}

bool SD_TC::mkdir(const char* path) {
  return sd.mkdir(path);
}

File SD_TC::open(const char* path, oflag_t oflag) {
  return sd.open(path, oflag);
}

void SD_TC::printRootDirectory() {
  sd.ls(LS_DATE | LS_SIZE | LS_R);
}

void SD_TC::listRootToBuffer(void (*callWhenFull)(char* buffer)) {
  const char path[] PROGMEM = "/";
  File root = SD_TC::instance()->open(path);
  if (!root) {
    serial(F("SD_TC open() failed"));
    return;
  }
  root.rewind();
  recursiveDir(root, callWhenFull);
  root.close();
}

void SD_TC::recursiveDir(File& dir, void (*callWhenFull)(char* buffer), byte tabulation) {
  File currFile;
  char fileName[13];  // Is it only 8 characters max, plus null term? 13 minimum per documentation
  char line[300];      // Each line shouldn't be more than 30 characters long

  #ifndef MOCK_PINS_COUNT
  while (currFile.openNext(&dir, O_READ)) {
    if (!currFile.isHidden()) {
      memset(line, ' ', sizeof(line));
      currFile.getName(fileName, sizeof(fileName));
      // Write line (either if directory or not) if size - bytes > 30
      for (int i = 0; i < tabulation; i++) {
        line[i] = '\t';
      }
      if (currFile.isDir()) {
        snprintf_P(line + tabulation, sizeof(line), (PGM_P)F("%s/\n"), fileName);
        //callWhenFull(line);
        recursiveDir(currFile, callWhenFull, tabulation + 1);
      } else {
        snprintf_P(line + tabulation, sizeof(line), (PGM_P)F("%s\t%6u bytes\n"), fileName, currFile.fileSize());
        callWhenFull(line);
      }
    }
    currFile.close();
  }
  #else
  char notImplemented[] PROGMEM = "Root directory not supported by CI framework.";
  callWhenFull(notImplemented);
  #endif
}

void SD_TC::todaysDataFileName(char* path, int size) {
  DateTime_TC now = DateTime_TC::now();
  snprintf_P(path, size, (PGM_P)F("%4i%02i%02i.csv"), now.year(), now.month(), now.day());
  COUT(path);
}
