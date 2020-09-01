void RecursiveRm(File dir, String tempPath) {
  Serial.println(F("CURRENT TEMPPATH: "));
  Serial.println(tempPath);
  while (true) {
    File entry = dir.openNextFile();
    String local_path;

    Serial.println(F("CURRENT ENTRY: "));
    Serial.println(entry.name());
    if (entry) {
      if (entry.isDirectory()) {
        local_path = tempPath + "/" + entry.name() + "/" + '\0';
        char folder_buf[local_path.length()];
        local_path.toCharArray(folder_buf, local_path.length());
        RecursiveRm(entry, folder_buf);

        if (SD.rmdir(folder_buf)) {
          Serial.print(F("Deleted folder "));
          Serial.println(folder_buf);
        } else {
          Serial.print(F("Unable to delete folder "));
          Serial.println(folder_buf);
        }
      } else {
        local_path = tempPath + entry.name() + '\0';
        char char_buf[local_path.length()];
        local_path.toCharArray(char_buf, local_path.length());

        if (SD.remove(char_buf)) {
          Serial.print(F("Deleted "));
          Serial.println(local_path);
        } else {
          Serial.print(F("Failed to delete "));
          Serial.println(local_path);
        }
      }
      entry.close();
    } else {
      // break out of recursion
      break;
    }
  }
  // delete specified root dir
  if (SD.rmdir(tempPath)) {
    Serial.print(F("Deleted root dir: "));
    Serial.println(tempPath);
  } else {
    Serial.print(F("Failed to delete root dir: "));
    Serial.println(tempPath);
  }
}
