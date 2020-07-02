#ifndef __MarceauSettings_h__
#define __MarceauSettings_h__

#define EEPROM_OFFSET 0
#define MAGIC_BYTE_1 0xF0
#define MAGIC_BYTE_2 0x0D
#define SETTINGS_VERSION 1

struct MarceauSettings {
  uint8_t      settingsVersion;
#ifdef ESP8266
  char         sta_ssid[32];
  char         sta_pass[64];
  bool         sta_dhcp;
  uint32_t     sta_fixedip;
  uint32_t     sta_fixedgateway;
  uint32_t     sta_fixednetmask;
  uint32_t     sta_fixeddns1;
  uint32_t     sta_fixeddns2;
  char         ap_ssid[32];
  char         ap_pass[64];
  bool         discovery;
#endif
};

#endif
