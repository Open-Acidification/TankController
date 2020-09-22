void HandleSeries(char* body, EthernetClient client) {
  StaticJsonDocument<1000> doc;
  DeserializationError error = deserializeJson(doc, body);
  char goal_record_string[GOAL_RECORD_LENGTH];
  memset(goal_record_string, 0, GOAL_RECORD_LENGTH);
  if (error) {
    PrintHeader(client, 400);

    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
  } else {
    PrintHeader(client, 200);

    int counter = 0;
    File series_file;
    JsonObject ph_object = doc["pH"];
    JsonArray ph_value_json_array = ph_object["value"];
    JsonArray ph_time_json_array = ph_object["time"];
    ph_interval = ph_object["interval"];
    ph_delay = ph_object["delay"];
    ph_series_size = ph_value_json_array.size();
    long ph_value_array[ph_series_size];
    long ph_time_array[ph_series_size];

    // validate pH series
    bool ph_series_is_valid = true;
    if (ph_value_json_array.size() != ph_time_json_array.size()) {
      client.println(F("pH SERIES NOT SAME SIZE"));
      ph_series_is_valid = false;
    }
    long ph_time_last = GetLastJsonArrayValue(ph_time_json_array);
    if (ph_time_last > ph_interval) {
      client.println(F("ph_time_last GREATER THAN ph_interval"));
      ph_series_is_valid = false;
    }

    if (ph_series_is_valid) {
      ph_series_pointer = 0;
      EepromWriteDouble(PH_INTERVAL_ADDRESS, ph_interval);
      EepromWriteDouble(PH_DELAY_ADDRESS, ph_delay);
      EepromWriteDouble(PH_SERIES_SIZE_ADDRESS, ph_series_size);
      EepromWriteDouble(PH_SERIES_POINTER_ADDRESS, ph_series_pointer);
      pinMode(10, OUTPUT);
      digitalWrite(10, HIGH);
      SD.remove("pv.txt");
      series_file = SD.open("pv.txt", FILE_WRITE);  // pH values
      client.println(F("pH values: "));
      for (JsonVariant v : ph_value_json_array) {
        ph_value_array[counter++] = v.as<long>();
        itoa(ph_value_array[counter - 1], goal_record_string, 10);
        for (strlen(goal_record_string); strlen(goal_record_string) < GOAL_RECORD_LENGTH;) {
          strcat(goal_record_string, " ");
        }
        client.println(goal_record_string);
        series_file.println(goal_record_string);
      }
      series_file.close();

      counter = 0;
      SD.remove("/pt.txt");
      series_file = SD.open("/pt.txt", FILE_WRITE);  // pH times
      client.println(F("pH times: "));
      memset(goal_record_string, 0, GOAL_RECORD_LENGTH);
      for (JsonVariant v : ph_time_json_array) {
        ph_time_array[counter++] = v.as<long>();
        itoa(ph_time_array[counter - 1], goal_record_string, 10);
        for (strlen(goal_record_string); strlen(goal_record_string) < GOAL_RECORD_LENGTH;) {
          strcat(goal_record_string, " ");
        }
        client.println(goal_record_string);
        series_file.println(goal_record_string);
      }
      series_file.close();
      client.println(F("pH interval: "));
      client.println(ph_interval);
      client.println(F("pH delay: "));
      client.println(ph_delay);
      client.print(F("ph series size: "));
      client.println(ph_series_size);
    } else {
      client.println(F("pH SERIES INVALID"));
    }

    JsonObject temp_object = doc["temp"];
    JsonArray temp_value_json_array = temp_object["value"];
    JsonArray temp_time_json_array = temp_object["time"];
    temp_interval = temp_object["interval"];
    temp_delay = temp_object["delay"];
    temp_series_size = temp_value_json_array.size();
    long temp_interval = temp_object["interval"];
    long temp_delay = temp_object["delay"];
    int temp_series_size = temp_value_json_array.size();
    long temp_value_array[temp_series_size];
    long temp_time_array[temp_series_size];

    // validate temp series
    bool temp_series_is_valid = true;
    if (temp_value_json_array.size() != temp_time_json_array.size()) {
      client.println(F("temp SERIES NOT SAME SIZE"));
      temp_series_is_valid = false;
    }
    long temp_time_last = GetLastJsonArrayValue(temp_time_json_array);
    if (temp_time_last > temp_interval) {
      client.println(F("temp_time_last GREATER THAN temp_interval"));
      temp_series_is_valid = false;
    }

    if (temp_series_is_valid) {
      counter = 0;
      temp_series_pointer = 0;
      EepromWriteDouble(TEMP_INTERVAL_ADDRESS, temp_interval);
      EepromWriteDouble(TEMP_DELAY_ADDRESS, temp_delay);
      EepromWriteDouble(TEMP_SERIES_SIZE_ADDRESS, temp_series_size);
      EepromWriteDouble(TEMP_SERIES_POINTER_ADDRESS, temp_series_pointer);
      SD.remove("/tv.txt");
      series_file = SD.open("/tv.txt", FILE_WRITE);  // temperature values
      client.println(F("temp values: "));
      memset(goal_record_string, 0, GOAL_RECORD_LENGTH);
      for (JsonVariant v : temp_value_json_array) {
        temp_value_array[counter++] = v.as<long>();
        itoa(temp_value_array[counter - 1], goal_record_string, 10);
        for (strlen(goal_record_string); strlen(goal_record_string) < GOAL_RECORD_LENGTH;) {
          strcat(goal_record_string, " ");
        }
        client.println(goal_record_string);
        series_file.println(goal_record_string);
      }
      series_file.close();

      counter = 0;
      SD.remove("/tt.txt");
      series_file = SD.open("/tt.txt", FILE_WRITE);  // temperature times
      client.println(F("temp values: "));
      memset(goal_record_string, 0, GOAL_RECORD_LENGTH);
      for (JsonVariant v : temp_time_json_array) {
        temp_time_array[counter++] = v.as<long>();
        itoa(temp_time_array[counter - 1], goal_record_string, 10);
        for (strlen(goal_record_string); strlen(goal_record_string) < GOAL_RECORD_LENGTH;) {
          strcat(goal_record_string, " ");
        }
        client.println(goal_record_string);
        series_file.println(goal_record_string);
      }
      series_file.close();
      client.println(F("temp interval: "));
      client.println(temp_interval);
      client.println(F("temp delay: "));
      client.println(temp_delay);
      client.print(F("temp series size: "));
      client.println(temp_series_size);
    } else {
      client.println(F("temp SERIES INVALID"));
    }
  }
}
