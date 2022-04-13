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

void SD_TC::writePhPoints(const char* line) {
  float pHPoint = std::stof(line);
  File file = sd.open("arb_pH_points", O_CREAT | O_WRONLY);
  file.write(line);
  file.close();
}

const char * SD_TC::readTextFileLine(const char * path, char* bytes, uint32_t seekTo) {
  float nextArbRightPoint;
  File file = open(path, O_RDONLY);
  file.seek(seekTo);
  char c = file.read();

  int i = 0;
  while(c != '\n') {
    bytes[i++] = c;
    c = file.read();
  }
  uint32_t position = file.position();
  float pHPoint = std::stof((const char *) bytes);
  return ( char *) bytes;
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
    if (!hasHadError) {  // output to serial once if file fails to open
      hasHadError = true;
      serial(F("Unable to open file: \"%s\""), path);
      COUT("Unable to open file: \"" << path << "\"");
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

// uint32_t SD_TC::position() {
//   return file.position();
// }

File SD_TC::open(const char* path, oflag_t oflag) {
  return sd.open(path, oflag);
}

void SD_TC::printRootDirectory() {
  sd.ls(LS_DATE | LS_SIZE | LS_R);
}

void SD_TC::todaysDataFileName(char* path, int size) {
  DateTime_TC now = DateTime_TC::now();
  snprintf_P(path, size, (PGM_P)F("%4i%02i%02i.csv"), now.year(), now.month(), now.day());
  COUT(path);
}
