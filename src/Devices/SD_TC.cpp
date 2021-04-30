#include "SD_TC.h"

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
  snprintf(path, sizeof(path), "/log/%4i/%02i/%02i.txt", now.year(), now.month(), now.day());
  appendDataToPath(data, path);
}

void printEntry(File entry, String parentPath) {
  int depth = 0;
  for (int i = 1; i < parentPath.length(); ++i) {
    if (parentPath[i] == '/') {
      ++depth;
    }
  }
  char tabs[] = "\t\t\t\t\t\t\t\t";
  if (depth < strlen(tabs)) {
    tabs[depth] = '\0';
  }
  if (entry.isDirectory()) {
    Serial_TC::instance()->printf((const char*)F("%s%s/"), tabs, entry.name());
  } else {
    // files have sizes, directories do not
    Serial_TC::instance()->printf((const char*)F("%s%s\t\t%i"), tabs, entry.name(), entry.size());
  }
}

/**
 * print the root directory and all subdirectories
 */
void SD_TC::printRootDirectory() {
  visit(printEntry);
}

String SD_TC::todaysDataFileName() {
  DateTime_TC now = DateTime_TC::now();
  char path[30];
  snprintf(path, sizeof(path), "/data/%4i/%02i/%02i.txt", now.year(), now.month(), now.day());
  return String(path);
}

void SD_TC::visit(visitor pFunction) {
  File root = open("/");
  visit(pFunction, root, "/");
}

void SD_TC::visit(visitor pFunction, File dir, String parentPath) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;  // no more files
    }
    pFunction(entry, parentPath);
    if (entry.isDirectory()) {
      visit(pFunction, entry, parentPath + entry.name() + "/");
    }
    entry.close();
  }
}
