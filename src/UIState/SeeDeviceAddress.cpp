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

int SeeDeviceAddress::writeToBuffer(char* buffer, int size, int line) {
  if (line) {
    byte* mac = Ethernet_TC::instance()->getMac();
    if (size > 17) {
      // Verbose, JSON-friendly
      return snprintf_P(buffer, size, (PGM_P)F("\"MAC\":\"%02X:%02X:%02X:%02X:%02X:%02X\""), mac[0], mac[1], mac[2],
                        mac[3], mac[4], mac[5]);
    } else {
      return snprintf_P(buffer, size, (PGM_P)F("%02X%02X:%02X%02X:%02X%02X"), mac[0], mac[1], mac[2], mac[3], mac[4],
                        mac[5]);
    }
  } else {
    IPAddress IP = Ethernet_TC::instance()->getIP();
    if (size > 17) {
      // Verbose, JSON-friendly
      return snprintf_P(buffer, size, (PGM_P)F("\"IPAddress\":\"%d.%d.%d.%d\""), IP[0], IP[1], IP[2], IP[3]);
    } else {
      return snprintf_P(buffer, size, (PGM_P)F("%03d.%03d.%03d.%03d"), IP[0], IP[1], IP[2], IP[3]);
    }
  }
}
void SeeDeviceAddress::start() {
  char buffer[17];
  // first line
  writeToBuffer(buffer, sizeof(buffer), 0);
  LiquidCrystal_TC::instance()->writeLine(buffer, 0);
  // second line
  writeToBuffer(buffer, sizeof(buffer), 1);
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}
