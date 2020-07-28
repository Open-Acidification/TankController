#include <string>

void handleRequest(EthernetClient client) {
  Serial.println("new client");

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
        Serial.println("Parsing request");

        Serial.println("header:");
        Serial.println(header);
        Serial.println("postData:");
        Serial.println(postData);

        requestType = strtok(header.c_str(), " ");
        endpoint = String(strtok(NULL, " "));
        requestProtocol = strtok(NULL, " ");

        Serial.println("requestType:");
        Serial.println(requestType);
        Serial.println("endpoint:");
        Serial.println(endpoint);
        Serial.println("requestProtocol:");
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
      handleConfig(postData.c_str(), client);
    } else if (endpoint.startsWith("/series")) {
      if (startsWith(requestType, "POST")) {
        // upload new timeseries
        Serial.println("POST TIMESERIES");
        handleSeries(postData.c_str(), client);
      } else if (startsWith(requestType, "GET")) {
        // return current timeseries
        Serial.println("GET TIMESERIES");
        handleGoal(client);
      } else {
        // wrong request type
        Serial.println("WRONG TIMESERIES");
        printHeader(client, 400);
      }
    } else if (endpoint.startsWith("/device")) {
      handleDevice(endpoint, client);
    } else if (endpoint.startsWith("/goal")) {
      handleGoal(client);
    } else if (endpoint.startsWith("/info")) {
      handleInfo(client);
    } else if (endpoint.startsWith("/data")) {
      handleData(endpoint, client);
    } else if (endpoint.startsWith("/lines")) {
      handleLines(endpoint, client);
    } else if (endpoint.startsWith("/test")) {
      printHeader(client, 200);
      File root = SD.open("/");
      printDirectoryToClient(root, 0, client);
    } else if (endpoint.startsWith("/mac")) {
      printHeader(client, 200);
      client.println(macstr);
    } else if (endpoint.startsWith("/time")) {
      printHeader(client, 200);
      DateTime now = rtc.now();
      char formattedSDString[50];
      char* timeFormat = "MM/DD/YYYY hh:mm:ss";
      char* formattedTime = now.toString(timeFormat);
      strcpy(formattedSDString, formattedTime);
      char buffer[100];
      snprintf(buffer, 100, "%d", tankid);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(temp, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(tempset, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(pH, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(phset, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(onTime, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(Kp, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(Ki, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(Kd, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      client.println(formattedSDString);
    } else {
      handleMisc(client);
    }
    return;
  }
}