template <int CMD_COUNT>
CmdProcessor<CMD_COUNT>::CmdProcessor(){
  this->in_process = false;
  current_id[0] = 0;
}

template <int CMD_COUNT>
bool CmdProcessor<CMD_COUNT>::addOutputHandler(jsonMsgHandler h){
  for(int i = 0; i< OUTPUT_HANDLER_COUNT; i++){
    if(outputHandlers[i] == NULL){
      outputHandlers[i] = h;
      return true;
    }
  }
  return false;
}

template <int CMD_COUNT>
void CmdProcessor<CMD_COUNT>::addCmd(const char cmd[], Fn func, bool immediate){
  if (cmd_counter == CMD_COUNT) {
    Serial.println("Too many commands defined");
    return;
  }
  _cmds[cmd_counter].cmd = cmd;
  _cmds[cmd_counter].func = func;
  _cmds[cmd_counter].immediate = immediate;
  cmd_counter++;
}

template <int CMD_COUNT>
boolean CmdProcessor<CMD_COUNT>::processMsg(char * msg){
  const char* cmd;
  const char* id;
  int cmd_num, i;
  StaticJsonBuffer<JSON_BUFFER_LENGTH> incomingBuffer;
  StaticJsonBuffer<JSON_BUFFER_LENGTH> outgoingBuffer;
  JsonObject& outMsg = outgoingBuffer.createObject();
  
  JsonObject& inMsg = incomingBuffer.parseObject(msg);
  if(inMsg.success()){
    // Extract the command
    if(!inMsg.containsKey("cmd")) return false;
    cmd = inMsg["cmd"];

    // Extract the ID
    if(inMsg.containsKey("id")){
      id = inMsg["id"];
    }else{
      id = "";
    }

    // Find the command
    cmd_num = -1;
    for(i = 0; i < cmd_counter; i++){
      if(!strcmp(cmd, _cmds[i].cmd)){
        cmd_num = i;
        break;
      }
    }

    // Process the command
    if(cmd_num >= 0){
      if(_cmds[cmd_num].immediate){
        (_cmds[cmd_num].func)(inMsg, outMsg);
        sendResponse("complete", outMsg, *id);
      }else{
        if(in_process){
          // the previous command hasn't finished, send an error
          outMsg["msg"] = "Previous command not finished";
          sendResponse("error", outMsg, *id);
        }else{
          (_cmds[cmd_num].func)(inMsg, outMsg);
          strcpy(current_id, (char*)id);
          in_process = true;
          sendResponse("accepted", outMsg, *id);
        }
      }
    }else{
      // the command isn't recognised, send an error
      outMsg["msg"] = "Command not recognised";
      sendResponse("error", outMsg, *id);
    }
  }else{
    //Error parsing
    outMsg["msg"] = "JSON parse error";
    sendResponse("error", outMsg, (const char &)"");
  }
  return true;
}

template <int CMD_COUNT>
void CmdProcessor<CMD_COUNT>::sendComplete(){
  if(in_process){
    in_process = false;
    StaticJsonBuffer<60> outBuffer;
    JsonObject& outMsg = outBuffer.createObject();
    sendResponse("complete", outMsg, *current_id);
  }
}

template <int CMD_COUNT>
void CmdProcessor<CMD_COUNT>::sendResponse(const char status[], ArduinoJson::JsonObject &outMsg, const char &id){
  if(strlen(&id)){
    outMsg["id"] = &id;
  }
  outMsg["status"] = status;
  for(int i = 0; i< OUTPUT_HANDLER_COUNT; i++){
    if(outputHandlers[i] != NULL){
      outputHandlers[i](outMsg);
    }
  }
}

template <int CMD_COUNT>
void CmdProcessor<CMD_COUNT>::notify(const char id[], ArduinoJson::JsonObject &outMsg){
  sendResponse("notify", outMsg, *id);
}
