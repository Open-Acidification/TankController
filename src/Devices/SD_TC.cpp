#include "Devices/SD_TC.h"

#include "DateTime_TC.h"
#include "Serial_TC.h"
#include "TC_util.h"

//  class variables
SDClass_TC* SDClass_TC::_instance = nullptr;

//  class methods
/**
 * accessor for singleton
 */
SDClass_TC* SDClass_TC::instance() {
  if (!_instance) {
    _instance = new SDClass_TC();
  }
  return _instance;
}

//  instance methods

/**
 * append data to a path
 */
void SDClass_TC::appendToPath(String data, String path) {
}

void SDClass_TC::appendToSerialLog(String data) {
  DateTime_TC now = DateTime_TC::now();
  char path[30];
  SDClass_TC::instance()->mkdir("/log/");
  sprintf(path, "/log/%4i/", now.year());
  SDClass_TC::instance()->mkdir(path);
  sprintf(path, "/log/%4i/%02i/", now.year(), now.month());
  SDClass_TC::instance()->mkdir(path);
  sprintf(path, "/log/%4i/%02i/%02i.txt", now.year(), now.month(), now.day());
  File file = SDClass_TC::instance()->open(path, FILE_WRITE);
  file.write(data.c_str(), data.size());
  file.write("\n", 1);
  file.close();
}

/**
 * print the root directory and all subdirectories
 */
void SDClass_TC::printRootDirectory() {
  File root = open("/");
  printDirectory(root, 0);
}

/**
 * print a specified directory with an indent
 */
void SDClass_TC::printDirectory(File dir, int numTabs) {
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
