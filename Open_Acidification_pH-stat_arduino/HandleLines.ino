void HandleLines(String endpoint, EthernetClient client) {
  // lines endpoint format: lines/year/month/day/hour
  String directory_name;
  if (endpoint.length() >= 6) {
    directory_name = endpoint.substring(6);  // remove "/lines/" prefix
  } else {
    directory_name = "/";
  }
  int slash_index = endpoint.lastIndexOf("/");
  String hour_name = endpoint.substring(slash_index);
  directory_name.concat(hour_name + ".txt");
  char* pch;
  int slashes = -1;
  pch = strtok(endpoint.c_str(), "/");
  while (pch != NULL) {
    pch = strtok(NULL, "/");
    slashes += 1;
  }
  switch (slashes) {
    case 4:
      // hour: return csv of the specific hour
      PrintHeader(client, 200);
      Serial.print(F("PRINTING LINE FILE: "));
      Serial.println(directory_name);
      long line_count = ReadLineFromSd(directory_name.c_str(), 0, 10);
      Serial.print(F("LINE COUNT: "));
      Serial.println(line_count);
      client.println(line_count);
      break;
    default:
      PrintHeader(client, 400);
      client.println(F("REQUIRES FULL YEAR/MONTH/DAY/HOUR SPECIFICATION"));
      break;
  }
}
