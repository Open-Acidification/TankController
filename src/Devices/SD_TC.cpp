#include "Devices/SD_TC.h"

#include "DateTime_TC.h"
#include "Serial_TC.h"
#define DEBUG 0
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
  assert(_instance == nullptr);
  serial("SD_TC::SD_TC()");
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
  begin(4);
}

/**
 * append data to a data log file
 */
void SD_TC::appendToDataLog(String header, String data) {
  String path = todaysDataFileName();
  COUT(path);
  if (!exists(path)) {
    appendDataToPath(header, path);
    COUT(header);
  }
  appendDataToPath(data, path);
  COUT(data);
}

/**
 * append data to a path
 */
void SD_TC::appendDataToPath(String data, String path) {
  int i = path.indexOf('/', 1);
  while (i > 0) {
    String s = path.substring(0, i);
    if (!SD.exists(s)) {
      if (!SD.mkdir(s.c_str())) {
        if (!hasHadError) {
          hasHadError = true;
          serial("Unable to create directory: \"%s\"", s.c_str());
          COUT("Unable to create directory: \"" << s.c_str() << "\"");
        }
        return;
      }
    }
    i = path.indexOf('/', i + 2);
  }
  File file = SD_TC::instance()->open(path, FILE_WRITE);
  if (file) {
    file.write(data.c_str(), data.length());
    file.write("\n", 1);
    file.close();
  } else {
    if (!hasHadError) {
      hasHadError = true;
      serial("Unable to open file: \"%s\"", path.c_str());
      COUT("Unable to open file: \"" << path.c_str() << "\"");
      return;
    }
  }
}

/**
 * append data to a serial log file
 */
void SD_TC::appendToSerialLog(String data) {
  DateTime_TC now = DateTime_TC::now();
  char path[30];
  snprintf(path, sizeof(path), "/log/%4i/%02i/%02i.txt", now.year(), now.month(), now.day());
  appendDataToPath(data, path);
}

void printEntry(File entry, String parentPath) {
  size_t depth = 0;
  for (size_t i = 1; i < parentPath.length(); ++i) {
    if (parentPath[i] == '/') {
      ++depth;
    }
  }
  char tabs[] = "- - - - - - - - ";
  if (depth * 2 < strlen(tabs)) {
    tabs[depth * 2] = '\0';
  }
  if (entry.isDirectory()) {
    serial("%s%s/", tabs, entry.name());
  } else {
    // files have sizes, directories do not
    serial("%s%12s (%6u)", tabs, entry.name(), entry.size());
  }
}

/**
 * print the root directory and all subdirectories
 */
void SD_TC::printRootDirectory() {
  serial("SD_TC::printRootDirectory() - start");
  visit(printEntry);
  serial("SD_TC::printRootDirectory() - end");
}

String SD_TC::todaysDataFileName() {
  DateTime_TC now = DateTime_TC::now();
  char path[30];
  snprintf(path, sizeof(path), "/data/%4i/%02i/%02i.txt", now.year(), now.month(), now.day());
  COUT(path);
  return String(path);
}

void SD_TC::visit(visitor pFunction) {
  File root = open("/");
  if (root) {
    visit(pFunction, root, "/");
  } else {
    serial("Unable to open root directory of SD card!");
  }
}

void SD_TC::visit(visitor pFunction, File dir, String parentPath) {
  int i = 0;
  while (i++ < 100) {
    File entry = dir.openNextFile();
    if (!entry) {
      return;  // no more files
    }
    pFunction(entry, parentPath);
    if (entry.isDirectory()) {
      visit(pFunction, entry, parentPath + entry.name() + "/");
    }
    entry.close();
  }
  serial("Stopped after 100 entries");
}
