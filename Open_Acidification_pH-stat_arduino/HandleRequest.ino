#include <string>

void HandleRequest(EthernetClient client) {
  Serial.println(F("new client"));

  // an http request ends with a blank line
  boolean current_line_is_blank = true;
  boolean header_finished = false;
  String post_data;
  String header;
  String request_type;
  String endpoint;
  String request_protocol;
  while (client.connected()) {
    while (client.available()) {
      char c = client.read();
      // read first line as header contains endpoint and request type information
      if (!header_finished) {
        header.concat(c);
      }
      // if you've gotten to the end of the line (received a newline
      // character) and the line is blank, the http request has ended,
      // so you can send a reply
      if (c == '\n' && current_line_is_blank) {
        // Here is where the POST data is.
        while (client.available()) {
          char c = client.read();
          Serial.write(c);
          post_data.concat(c);
        }

        Serial.println();
        Serial.println(F("Parsing request"));

        Serial.println(F("header:"));
        Serial.println(header);
        Serial.println(F("post_data:"));
        Serial.println(post_data);

        request_type = String(strtok(header.c_str(), " "));
        endpoint = String(strtok(NULL, " "));
        request_protocol = String(strtok(NULL, " "));

        Serial.println(F("request_type:"));
        Serial.println(request_type);
        Serial.println(F("endpoint:"));
        Serial.println(endpoint);
        Serial.println(F("request_protocol:"));
        Serial.println(request_protocol);
        break;
      } else if (c == '\n') {
        // you're starting a new line
        current_line_is_blank = true;
        header_finished = true;
      } else if (c != '\r') {
        // you've gotten a character on the current line
        current_line_is_blank = false;
      }
    }

    // parse and handle request based on endpoint
    if (endpoint.startsWith("/config")) {
      HandleConfig(post_data.c_str(), client);
    } else if (endpoint.startsWith("/series")) {
      if (request_type.startsWith("POST")) {
        // upload new timeseries
        Serial.println(F("POST TIMESERIES"));
        HandleSeries(post_data.c_str(), client);
      } else if (request_type.startsWith("GET")) {
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
      client.println(mac_str);
    } else if (endpoint.startsWith("/time")) {
      PrintHeader(client, 200);
      DateTime now = rtc.now();
      char formatted_sd_string[50];
      char* time_format = "MM/DD/YYYY hh:mm:ss";
      char* formatted_time = now.toString(time_format);
      strcpy(formatted_sd_string, formatted_time);
      char buffer[100];
      snprintf(buffer, 100, "%d", tank_id);
      strcat(formatted_sd_string, ",");
      strcat(formatted_sd_string, buffer);
      FormatDouble(temp, 3, buffer);
      strcat(formatted_sd_string, ",");
      strcat(formatted_sd_string, buffer);
      FormatDouble(temp_set, 3, buffer);
      strcat(formatted_sd_string, ",");
      strcat(formatted_sd_string, buffer);
      FormatDouble(pH, 3, buffer);
      strcat(formatted_sd_string, ",");
      strcat(formatted_sd_string, buffer);
      FormatDouble(ph_set, 3, buffer);
      strcat(formatted_sd_string, ",");
      strcat(formatted_sd_string, buffer);
      FormatDouble(on_time, 3, buffer);
      strcat(formatted_sd_string, ",");
      strcat(formatted_sd_string, buffer);
      FormatDouble(Kp, 3, buffer);
      strcat(formatted_sd_string, ",");
      strcat(formatted_sd_string, buffer);
      FormatDouble(Ki, 3, buffer);
      strcat(formatted_sd_string, ",");
      strcat(formatted_sd_string, buffer);
      FormatDouble(Kd, 3, buffer);
      strcat(formatted_sd_string, ",");
      strcat(formatted_sd_string, buffer);
      client.println(formatted_sd_string);
    } else {
      HandleMisc(client);
    }
    return;
  }
}