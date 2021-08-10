#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include <vector>

#include "Devices/EthernetServer_TC.h"
/**
 * Trivial test to confirm that EthernetServer_TC compiles,
 * is of proper class, and has proper port
 */
unittest(test) {
  EthernetServer_TC* server = EthernetServer_TC::instance();
  assertEqual("EthernetServer_TC", server->className());
  assertEqual(80, server->getPort());
}

unittest_main()
