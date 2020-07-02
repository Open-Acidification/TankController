#ifdef ESP8266
#include "Arduino.h"
#include "MarceauWeb.h"

AsyncWebServer server(80);
extern t_webfiles webFiles[];
extern int webFileCount;

class MarceauRequestHandler: public AsyncWebHandler {
  public:
    MarceauRequestHandler(){}
    bool canHandle(AsyncWebServerRequest *request){
      return request->method() == HTTP_GET;
    }

    void handleRequest(AsyncWebServerRequest *request){
      uint8_t i;
      bool found = false;
      bool gzip = false;

      if(request->method() == HTTP_GET){
        String path = request->url();
        if(path.endsWith("/")) path += "index.html";
        
        // Loop through our files to find one that matches
        for(i=0; i<webFileCount; i++){

          if(path + ".gz"== webFiles[i].filename){
            found = true;
            gzip = true;
          }else if(path == webFiles[i].filename){
            found = true;
          }

          if(found){
            // If it's a match, send the file
            AsyncWebServerResponse *response = request->beginResponse_P(
              200,
              webFiles[i].mime,
              webFiles[i].content,
              webFiles[i].len
            );
            if(gzip) response->addHeader("Content-Encoding", "gzip");
            request->send(response);
            
            break;
          }
        }
        // If we get here, send a 404
        if(!found) request->send(404, "text/plain", "Not Found");
      }
    }
};

void MarceauWeb::begin() {
  FSInfo fs_info;
  bool res = SPIFFS.begin();

  if(res && SPIFFS.info(fs_info)){
    server.serveStatic("/a/", SPIFFS, "/")
          .setLastModified("Wed, 20 Dec 2017 18:35:00 GMT");
  }else{
    //TODO: Decide if we want to load a full image or individual files
    //SPIFFS.format();
  }

  server.addHandler(new MarceauRequestHandler());
	server.begin();
}

#endif
