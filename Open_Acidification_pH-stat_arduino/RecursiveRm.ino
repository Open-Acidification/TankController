void RecursiveRm(File dir, String tempPath) {
  Serial.println(F("CURRENT TEMPPATH: "));
  Serial.println(tempPath);
  while (true) {
    File entry = dir.openNextFile();
    String localPath;

    Serial.println(F("CURRENT ENTRY: "));
    Serial.println(entry.name());
    if (entry) {
      if (entry.isDirectory()) {
        localPath = tempPath + "/" + entry.name() + "/" + '\0';
        char folderBuf[localPath.length()];
        localPath.toCharArray(folderBuf, localPath.length());
        RecursiveRm(entry, folderBuf);

        if (SD.rmdir(folderBuf)) {
          Serial.print(F("Deleted folder "));
          Serial.println(folderBuf);
        } else {
          Serial.print(F("Unable to delete folder "));
          Serial.println(folderBuf);
        }
      } else {
        localPath = tempPath + entry.name() + '\0';
        char charBuf[localPath.length()];
        localPath.toCharArray(charBuf, localPath.length());

        if (SD.remove(charBuf)) {
          Serial.print(F("Deleted "));
          Serial.println(localPath);
        } else {
          Serial.print(F("Failed to delete "));
          Serial.println(localPath);
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
