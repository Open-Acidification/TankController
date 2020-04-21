#ifndef __Marceau_h__
#define __Marceau_h__

#include "Arduino.h"
#include "lib/CmdProcessor.h"
#include "lib/ArduinoJson/ArduinoJson.h"
#include "lib/MarceauSettings.h"
#include "lib/webFiles.h"
#include <EEPROM.h>

#ifdef ESP8266
#include "lib/MarceauWifi.h"
#include "lib/MarceauWeb.h"
#include "lib/MarceauWebSocket.h"
#endif

#define SERIAL_BUFFER_LENGTH 180

#ifdef ESP8266
#define BUILTIN_CMDS 7
#else
#define BUILTIN_CMDS 2
#endif

typedef void (* Fn) (ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
typedef void (* cb) ();

class MarceauBase
{
  public:
#ifdef ESP8266
    virtual void handleWsMsg(char * msg);
    virtual void getConfig(ArduinoJson::JsonObject &inJson, ArduinoJson::JsonObject &outJson);
    virtual void setConfig(ArduinoJson::JsonObject &inJson, ArduinoJson::JsonObject &outJson);
    virtual void resetConfig(ArduinoJson::JsonObject &inJson, ArduinoJson::JsonObject &outJson);
#endif
};

template <uint8_t CMD_COUNT>
class Marceau: public MarceauBase{
  public:
    Marceau();
    void begin();
    void enableSerial(Stream &s);
    void loop();
    void addCmd(const char cmd[], Fn func, bool immediate);
    void cmdComplete();
    void resetSettings();
    void notify(char * status, ArduinoJson::JsonObject &msg);
    MarceauSettings settings;
    boolean blocking;
#ifdef ESP8266
    MarceauWifi wifi;
    void enableWifi();
    void setHostname(char * hostname);
    void setDefaultAPName(char * apname);
    virtual void handleWsMsg(char * msg);
    virtual void getConfig(ArduinoJson::JsonObject &inJson, ArduinoJson::JsonObject &outJson);
    virtual void setConfig(ArduinoJson::JsonObject &inJson, ArduinoJson::JsonObject &outJson);
    virtual void resetConfig(ArduinoJson::JsonObject &inJson, ArduinoJson::JsonObject &outJson);
#endif
  private:
    void wait();
    void initSettings();
    void saveSettings();
    void version(char);
    void initCmds();
    void serialHandler();
    Stream *_s;
    unsigned long last_char;
    char serial_buffer[SERIAL_BUFFER_LENGTH];
    int serial_buffer_pos;
    bool wifiEnabled;
    bool serialEnabled;
    CmdProcessor<CMD_COUNT+BUILTIN_CMDS> p;
#ifdef ESP8266
    char * defaultAPName;
    void networkNotifier();
    void wifiScanNotifier();
    MarceauWeb webServer;
    MarceauWebSocket socketServer;
#endif
};

#include "Marceau.tpp"

#endif
