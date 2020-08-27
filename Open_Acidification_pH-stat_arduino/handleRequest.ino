#include <string>

void HandleRequest(EthernetClient client) {
  Serial.println(F("new client"));

  // an http request ends with a blank line
  boolean currentLineIsBlank = true;
  boolean headerFinished = false;
  String postData;
  String header;
  char* requestType;
  String endpoint;
  char* requestProtocol;
  while (client.connected()) {
    while (client.available()) {
      char c = client.read();
      // read first line as header contains endpoint and request type information
      if (!headerFinished) {
        header.concat(c);
      }
      // if you've gotten to the end of the line (received a newline
      // character) and the line is blank, the http request has ended,
      // so you can send a reply
      if (c == '\n' && currentLineIsBlank) {
        // Here is where the POST data is.
        while (client.available()) {
          char c = client.read();
          Serial.write(c);
          postData.concat(c);
        }

        Serial.println();
        Serial.println(F("Parsing request"));

        Serial.println(F("header:"));
        Serial.println(header);
        Serial.println(F("postData:"));
        Serial.println(postData);

        requestType = strtok(header.c_str(), " ");
        endpoint = String(strtok(NULL, " "));
        requestProtocol = strtok(NULL, " ");

        Serial.println(F("requestType:"));
        Serial.println(requestType);
        Serial.println(F("endpoint:"));
        Serial.println(endpoint);
        Serial.println(F("requestProtocol:"));
        Serial.println(requestProtocol);
        break;
      } else if (c == '\n') {
        // you're starting a new line
        currentLineIsBlank = true;
        headerFinished = true;
      } else if (c != '\r') {
        // you've gotten a character on the current line
        currentLineIsBlank = false;
      }
    }

    // parse and handle request based on endpoint
    if (endpoint.startsWith("/config")) {
      HandleConfig(postData.c_str(), client);
    } else if (endpoint.startsWith("/series")) {
      if (startsWith(requestType, "POST")) {
        // upload new timeseries
        Serial.println(F("POST TIMESERIES"));
        HandleSeries(postData.c_str(), client);
      } else if (startsWith(requestType, "GET")) {
        // return current timeseries
        Serial.println(F("GET TIMESERIES"));
        HandleGoal(client);
      } else {
        // wrong request type
        Serial.println(F("WRONG TIMESERIES"));
        PrintHeader(client, 400);
      }
    } else if (endpoint.startsWith("/device")) {
      HandleDevice(endpoint, client);
    } else if (endpoint.startsWith("/goal")) {
      HandleGoal(client);
    } else if (endpoint.startsWith("/info")) {
      HandleInfo(client);
    } else if (endpoint.startsWith("/data")) {
      HandleData(endpoint, client);
    } else if (endpoint.startsWith("/lines")) {
      HandleLines(endpoint, client);
    } else if (endpoint.startsWith("/test")) {
      PrintHeader(client, 200);
      File root = SD.open("/");
      PrintDirectoryToClient(root, 0, client);
    } else if (endpoint.startsWith("/mac")) {
      PrintHeader(client, 200);
      client.println(macstr);
    } else if (endpoint.startsWith("/time")) {
      PrintHeader(client, 200);
      DateTime now = rtc.now();
      char formattedSDString[50];
      char* timeFormat = "MM/DD/YYYY hh:mm:ss";
      char* formattedTime = now.toString(timeFormat);
      strcpy(formattedSDString, formattedTime);
      char buffer[100];
      snprintf(buffer, 100, "%d", tankid);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      FormatDouble(temp, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      FormatDouble(tempset, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      FormatDouble(pH, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      FormatDouble(phset, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      FormatDouble(onTime, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      FormatDouble(Kp, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      FormatDouble(Ki, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      FormatDouble(Kd, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      client.println(formattedSDString);
    } else {
      HandleMisc(client);
    }
    return;
  }
}