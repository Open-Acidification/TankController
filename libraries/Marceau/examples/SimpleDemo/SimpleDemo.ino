#include <Marceau.h>

Marceau<2> marcel;
long completeTime = 0;

void testImmediateFn(ArduinoJson::JsonObject &input, ArduinoJson::JsonObject &output){
  output["msg"] = "1.2.3";
}

void testDelayedFn(ArduinoJson::JsonObject &input, ArduinoJson::JsonObject &output){
  completeTime = millis() + 2000;
}

void setup(){
  Serial.begin(57600);
  marcel.addCmd("version", testImmediateFn, true);
  marcel.addCmd("delay", testDelayedFn, false);
  marcel.enableSerial(Serial);
  marcel.begin();
}

void loop(){
  marcel.loop();
  if(completeTime != 0 && completeTime < millis()){
    marcel.cmdComplete();
    completeTime = 0;
  }
}