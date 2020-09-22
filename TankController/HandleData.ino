void HandleData(String endpoint, EthernetClient client) {
  // data endpoint format: data/year/month/day/hour
  String directory_name;
  if (endpoint.length() >= 5) {
    directory_name = endpoint.substring(5);  // remove "/data/" prefix
  } else {
    directory_name = "/";
  }
  long starting_line = 0;
  long num_lines = 0;
  long last_lines = 0;
  int last_index = -1;
  // check if there are url parameters
  int url_parameter_index = directory_name.indexOf("?");
  if (url_parameter_index > -1) {
    String url_parameter = directory_name.substring(url_parameter_index);
    directory_name = directory_name.substring(0, url_parameter_index);
    last_index = url_parameter.indexOf("last");
    if (last_index > -1) {  // print last lines
      last_lines = url_parameter.substring(last_index + 5).toInt();
      Serial.println(F("========"));
      Serial.println(last_lines);
      Serial.println(F("========"));
    } else {  // print specified lines
      int startIndex = url_parameter.indexOf("start");
      int numIndex = url_parameter.indexOf("num");
      starting_line = url_parameter.substring(startIndex + 6, numIndex - 1).toInt();
      num_lines = url_parameter.substring(numIndex + 4).toInt();
      Serial.println(F("========"));
      Serial.println(starting_line);
      Serial.println(num_lines);
      Serial.println(F("========"));
    }
  }
  char* pch;
  int slashes = -1;
  pch = strtok(endpoint.c_str(), "/");
  while (pch != NULL) {
    pch = strtok(NULL, "/");
    slashes += 1;
  }
  File dir = SD.open(directory_name);
  Serial.print(F("HOW MANY SLASHES: "));
  Serial.println(slashes);
  switch (slashes) {
    case 0:  // data: return years available
    case 1:  // year: return months available
    case 2:  // month: return days available
    case 3:  // day: return hours available
      Serial.println(F("START BUILDING JSON"));
      PrintCurrentLevelDirectories(dir, client, slashes);
      break;
    case 4:
      String file_name = "/" + directory_name.substring(3, 5) + directory_name.substring(6, 8) + directory_name.substring(9, 11) + directory_name.substring(12, 14) + ".txt";
      directory_name.concat(file_name);
      if (url_parameter_index > -1 && last_index > -1) {  // last lines: print last_lines in specified csv
        Serial.print(F("PRINTING LAST LINES FROM FILE: "));
        Serial.println(directory_name);
        PrintLastLines(directory_name, client, last_lines);
      } else if (url_parameter_index > -1) {  // specified lines: print num_lines from starting_line in specified csv
        Serial.print(F("PRINTING SPECIFIED LINES FROM FILE: "));
        Serial.println(directory_name);
        PrintSpecifiedLines(directory_name, client, starting_line, num_lines);
      } else {  // hour: return csv of the specific hour
        Serial.print(F("PRINTING FILE: "));
        Serial.println(directory_name);
        PrintFileInDirectory(directory_name, client);
      }
      break;
    default:
      PrintHeader(client, 404);
      break;
  }
  dir.close();
}
