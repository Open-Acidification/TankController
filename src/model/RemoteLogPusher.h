#pragma once
#include <Arduino.h>

class RemoteLogPusher {
public:
  // class methods
  static RemoteLogPusher *instance();

  // instance methods
  void loop();
  void pushSoon();

private:
  // class variables
  static RemoteLogPusher *_instance;
};
