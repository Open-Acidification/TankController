#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "AlertPusher.h"
#include "DateTime_TC.h"
#include "SD_TC.h"
#include "TC_util.h"
#include "TankController.h"

unittest_setup() {
  GODMODE()->reset();
  Ethernet_TC* client = Ethernet_TC::instance();
  // client->set
}

unittest_teardown() {
  SD_TC::instance()->format();
}

unittest(singleton) {
  AlertPusher* thing1 = AlertPusher::instance();
  AlertPusher* thing2 = AlertPusher::instance();
  assertTrue(thing1 != nullptr);
  assertEqual(thing1, thing2);
}

unittest(sendPostRequest) {
  SD_TC* sd = SD_TC::instance();

  // write data
  sd->setAlertFileName("Tank1");
  sd->writeAlert("line 1\nand 2\nline 3\n");

  // post line 2
}

unittest_main()
