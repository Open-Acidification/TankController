#pragma once

#include <Arduino.h>

/**
 * JSONBuilder builds a text-based JSON file.
 */

const int BUFFER_SIZE = 256;

class JSONBuilder {
public:
  JSONBuilder() {
    buffer = new char[BUFFER_SIZE];
  }
  ~JSONBuilder() {
    delete[] buffer;
  }

  int buildCurrentValues();
  char* bufferPtr() {
    return buffer;
  }
  int bytesFull() {
    return bytes;
  }

private:
  char* buffer = nullptr;
  int bytes = 0;  // Excludes '\0' at end
};
