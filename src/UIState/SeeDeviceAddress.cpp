/**
 * SeeDeviceAddress.cpp
 */

#include "SeeDeviceAddress.h"

#include <avr/wdt.h>

#include "Devices/Ethernet_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Ethernet.h"

void SeeDeviceAddress::handleKey(char key) {
  switch (key) {
    case '#':  // Disable Watchdog Timer (is reboot from this?)
      wdt_disable();
      break;
    case 'B':  // Infinite loop to test Watchdog Timer
      wdt_disable();
      wdt_enable(WDTO_15MS);
      do {
      } while (true);
      break;
    case 'C':  // Reset MAC address
      Ethernet_TC::instance()->readMac(true);
      start();
      break;
    default:
      returnToMainMenu();
      break;
  };
}

void SeeDeviceAddress::start() {
  char line[17];
  // first line
  IPAddress IP = Ethernet_TC::instance()->getIP();
  snprintf_P(line, sizeof(line), (PGM_P)F("%d.%d.%d.%d"), IP[0], IP[1], IP[2], IP[3]);
  LiquidCrystal_TC::instance()->writeLine(line, 0);
  // second line
  byte* mac = Ethernet_TC::instance()->getMac();
  snprintf_P(line, sizeof(line), (PGM_P)F("%02X%02X:%02X%02X:%02X%02X"), mac[0], mac[1], mac[2], mac[3], mac[4],
             mac[5]);
  LiquidCrystal_TC::instance()->writeLine(line, 1);
}
