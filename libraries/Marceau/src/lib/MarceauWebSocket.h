#ifndef __MirobotWebSocket_h__
#define __MirobotWebSocket_h__

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "lib/ESPAsyncTCP/ESPAsyncTCP.h"
#include "lib/ESPAsyncWebServer/ESPAsyncWebServer.h"
#include "lib/ArduinoJson/ArduinoJson.h"

#ifdef AVR
#define JSON_BUFFER_LENGTH 128
#endif //AVR

#ifdef ESP8266
#define JSON_BUFFER_LENGTH 512
#endif //ESP8266

typedef void (* dataHandler) (char *);

class MarceauWebSocket {
  public:
    void begin();
    void onMsg(dataHandler h);
    static void sendWebSocketMsg(ArduinoJson::JsonObject &);
};

#endif
