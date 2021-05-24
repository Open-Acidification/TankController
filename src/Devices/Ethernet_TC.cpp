#include "Ethernet_TC.h"

#include "Serial_TC.h"

Ethernet_TC *Ethernet_TC::_instance = nullptr;

// Establishes the Ethernet connection and sets class variables
Ethernet_TC::Ethernet_TC() {
  pinMode(IO_PIN, OUTPUT);
  digitalWrite(IO_PIN, HIGH);
  if (Ethernet.begin(mac) == 0) {
    serial("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, defaultIP);
    IP = defaultIP;
  }
}

Ethernet_TC *Ethernet_TC::instance() {
  if (_instance == nullptr) {
    _instance = new Ethernet_TC;
  }
  return _instance;
}

void Ethernet_TC::renewDHCPLease() {
  uint32_t current_millis = millis();
  if ((current_millis - previous_lease) >= LEASE_INTERVAL || current_millis < previous_lease) {
    Ethernet.maintain();
    previous_lease = current_millis;
  }
  numAttemptedDHCPReleases++;
}
