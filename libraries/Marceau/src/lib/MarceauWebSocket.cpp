#ifdef ESP8266
#include "Arduino.h"
#include "MarceauWebSocket.h"

AsyncWebServer wsServer(8899);
AsyncWebSocket ws("/websocket");

dataHandler handler = NULL;

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_DATA){
    //data packet
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len){
      //the whole message is in a single frame and we got all of it's data
      if(info->opcode == WS_TEXT){
        data[len] = 0;        
        if(handler) handler((char *)data);
      }
    }
  }
}

void MarceauWebSocket::begin() {
  ws.onEvent(onEvent);
  wsServer.addHandler(&ws);
	wsServer.begin();
}

void MarceauWebSocket::onMsg(dataHandler h){
  handler = h;
}

void MarceauWebSocket::sendWebSocketMsg(ArduinoJson::JsonObject &msg){
  char jsonBuff[JSON_BUFFER_LENGTH];
  msg.printTo(jsonBuff, sizeof(jsonBuff));
  ws.textAll(jsonBuff, strlen(jsonBuff));
}

#endif
