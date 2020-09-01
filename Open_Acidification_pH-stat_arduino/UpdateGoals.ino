void UpdateGoals() {
  long now = millis();

  long ph_time = now % ph_interval;

  long ph_time_current = ReadLineFromSd("pt.txt", ph_series_pointer, GOAL_RECORD_LENGTH);
  long ph_time_next = ReadLineFromSd("pt.txt", ph_series_pointer + 1 >= ph_series_size ? 0 : ph_series_pointer + 1, GOAL_RECORD_LENGTH);

  // we want to make sure ph_time is always between ph_time_current and ph_time_next
  if (ph_series_size) {
    while ((ph_time_next > ph_time_current) && (ph_time_next < ph_time)) {
      ph_series_pointer++;
      if (ph_series_pointer >= ph_series_size) {
        ph_series_pointer = 0;
      }
      ph_time_current = ReadLineFromSd("pt.txt", ph_series_pointer, GOAL_RECORD_LENGTH);
      ph_time_next = ReadLineFromSd("pt.txt", ph_series_pointer + 1 >= ph_series_size ? 0 : ph_series_pointer + 1, GOAL_RECORD_LENGTH);
    }
    if ((ph_time_next < ph_time_current) && (ph_time_current > ph_time)) {
      ph_series_pointer = 0;
      ph_time_current = ReadLineFromSd("pt.txt", ph_series_pointer, GOAL_RECORD_LENGTH);
      ph_time_next = ReadLineFromSd("pt.txt", ph_series_pointer + 1 >= ph_series_size ? 0 : ph_series_pointer + 1, GOAL_RECORD_LENGTH);
    }
  }
  EepromWriteDouble(PH_SERIES_POINTER_ADDRESS, ph_series_pointer);

  // interpolate ph goal
  long ph_value_current = ReadLineFromSd("pv.txt", ph_series_pointer, GOAL_RECORD_LENGTH);
  long ph_value_next;
  if (ph_series_pointer + 1 >= ph_series_size) {
    ph_value_next = ReadLineFromSd("pv.txt", 0, GOAL_RECORD_LENGTH);
    ph_time_next = ph_interval;
  } else {
    ph_value_next = ReadLineFromSd("pv.txt", ph_series_pointer + 1, GOAL_RECORD_LENGTH);
  }
  ph_set = ph_value_current + (ph_value_next - ph_value_current) * (ph_time - ph_time_current) / (ph_time_next - ph_time_current);

  // now do the same for temp
  long temp_time = now % temp_interval;

  long temp_time_current = ReadLineFromSd("tt.txt", temp_series_pointer, GOAL_RECORD_LENGTH);
  long temp_time_next = ReadLineFromSd("tt.txt", temp_series_pointer + 1 >= temp_series_size ? 0 : temp_series_pointer + 1, GOAL_RECORD_LENGTH);

  // we want to make sure temp_time is always between temp_time_current and temp_time_next
  if (temp_series_size) {
    while ((temp_time_next > temp_time_current) && (temp_time_next < temp_time)) {
      temp_series_pointer++;
      if (temp_series_pointer >= temp_series_size) {
        temp_series_pointer = 0;
      }
      temp_time_current = ReadLineFromSd("tt.txt", temp_series_pointer, GOAL_RECORD_LENGTH);
      temp_time_next = ReadLineFromSd("tt.txt", temp_series_pointer + 1 >= temp_series_size ? 0 : temp_series_pointer + 1, GOAL_RECORD_LENGTH);
    }
    if ((temp_time_next < temp_time_current) && (temp_time_current > temp_time)) {
      temp_series_pointer = 0;
      temp_time_current = ReadLineFromSd("tt.txt", temp_series_pointer, GOAL_RECORD_LENGTH);
      temp_time_next = ReadLineFromSd("tt.txt", temp_series_pointer + 1 >= temp_series_size ? 0 : temp_series_pointer + 1, GOAL_RECORD_LENGTH);
    }
  }
  EepromWriteDouble(TEMP_SERIES_POINTER_ADDRESS, temp_series_pointer);

  // interpolate temp goal
  long temp_value_current = ReadLineFromSd("tv.txt", temp_series_pointer, GOAL_RECORD_LENGTH);
  long temp_value_next;
  if (temp_series_pointer + 1 >= temp_series_size) {
    temp_value_next = ReadLineFromSd("tv.txt", 0, GOAL_RECORD_LENGTH);
    temp_time_next = temp_interval;
  } else {
    temp_value_next = ReadLineFromSd("tv.txt", temp_series_pointer + 1, GOAL_RECORD_LENGTH);
  }
  temp_set = temp_value_current + (temp_value_next - temp_value_current) * (temp_time - temp_time_current) / (temp_time_next - temp_time_current);
}
