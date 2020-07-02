#ifndef __CmdProcessor_h__
#define __CmdProcessor_h__

#include "Arduino.h"
#include "Marceau.h"
#include "./lib/ArduinoJson/ArduinoJson.h"

#define OUTPUT_HANDLER_COUNT 2

#ifdef AVR
#define JSON_BUFFER_LENGTH 128
#endif //AVR

#ifdef ESP8266
#define JSON_BUFFER_LENGTH 512
#endif //ESP8266

typedef void (* Fn) (ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
typedef void (* jsonMsgHandler) (ArduinoJson::JsonObject &);

struct Cmd {
  const char *cmd;
  Fn func;
  bool immediate;
};

template <int CMD_COUNT>
class CmdProcessor {
  public:
    CmdProcessor();
    void addCmd(const char cmd[], Fn func, bool immediate);
    bool addOutputHandler(jsonMsgHandler);
    void process();
    void notify(const char[], ArduinoJson::JsonObject &);
    void sendComplete();
    boolean processMsg(char * msg);
    int cmdCount();
    char * getCmdName(int);
    boolean in_process;
  private:
    boolean processLine();
    void processCmd(const char &cmd, const char &arg, const char &id);
    void runCmd(char &cmd, char &arg, char &id);
    void sendResponse(const char state[], ArduinoJson::JsonObject &, const char &id);
    char current_id[11];
    boolean processJSON();
    Cmd _cmds[CMD_COUNT];
    int cmd_counter;
    jsonMsgHandler outputHandlers[OUTPUT_HANDLER_COUNT];
};

#include "CmdProcessor.tpp"

#endif
