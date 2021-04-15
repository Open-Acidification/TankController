#include "Devices/SD_TC.h"

#include "Serial_TC.h"

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
