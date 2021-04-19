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
 * append data to a data log file
 */
void SD_TC::appendToDataLog(String header, String data) {
  String path = todaysDataFileName();
  if (!exists(path)) {
    appendDataToPath(header, path);
  }
  appendDataToPath(data, path);
}

/**
 * append data to a path
 */
void SD_TC::appendDataToPath(String data, String path) {
  int i = path.indexOf('/', 1);
  while (i > 0) {
    String s = path.substring(0, ++i);
    SD_TC::instance()->mkdir(s);
    i = path.indexOf('/', i);
  }
  File file = SD_TC::instance()->open(path, FILE_WRITE);
  file.write(data.c_str(), data.length());
  file.write("\n", 1);
  file.close();
}

/**
 * append data to a serial log file
 */
void SD_TC::appendToSerialLog(String data) {
  DateTime_TC now = DateTime_TC::now();
  char path[30];
  sprintf(path, "/log/%4i/%02i/%02i.txt", now.year(), now.month(), now.day());
  appendDataToPath(data, path);
}

/**
 * print the root directory and all subdirectories
 */
void SD_TC::printRootDirectory() {
  File root = open("/");
  printDirectory(root, 0);
}

/**
 * print a specified directory with an indent
 */
void SD_TC::printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }
    char tabs[] = "\t\t\t\t\t\t\t\t";
    if (numTabs < strlen(tabs)) {
      tabs[numTabs] = '\0';
    }
    if (entry.isDirectory()) {
      Serial_TC::instance()->printf((const char*)F("%s%s/"), tabs, entry.name());
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial_TC::instance()->printf((const char*)F("%s%s\t\t%i"), tabs, entry.name(), entry.size());
    }
    entry.close();
  }
}

String todaysDataFileName() {
  DateTime_TC now = DateTime_TC::now();
  char path[30];
  sprintf(path, "/data/%4i/%02i/%02i.txt", now.year(), now.month(), now.day());
  return String(path);
}
