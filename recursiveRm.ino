void recursiveRm(File dir, String tempPath) {
  Serial.println("CURRENT TEMPPATH: ");
  Serial.println(tempPath);
  while(true) {
    File entry =  dir.openNextFile();
    String localPath;

    Serial.println("CURRENT ENTRY: ");
    Serial.println(entry.name());
    if (entry) {
      if ( entry.isDirectory() )
      {
        localPath = tempPath + "/" + entry.name() + "/" + '\0';
        char folderBuf[localPath.length()];
        localPath.toCharArray(folderBuf, localPath.length() );
        recursiveRm(entry, folderBuf);


        if( SD.rmdir( folderBuf ) )
        {
          Serial.print("Deleted folder ");
          Serial.println(folderBuf);
        } 
        else
        {
          Serial.print("Unable to delete folder ");
          Serial.println(folderBuf);
        }
      } 
      else
      {
        localPath = tempPath + entry.name() + '\0';
        char charBuf[localPath.length()];
        localPath.toCharArray(charBuf, localPath.length() );

        if( SD.remove( charBuf ) )
        {
          Serial.print("Deleted ");
          Serial.println(localPath);
        } 
        else
        {
          Serial.print("Failed to delete ");
          Serial.println(localPath);
        }

      }
      entry.close();
    } 
    else {
      // break out of recursion
      break;
    }
  }
  // delete specified root dir
  if( SD.rmdir( tempPath ) )
  {
    Serial.print("Deleted root dir: ");
    Serial.println(tempPath);
  } 
  else
  {
    Serial.print("Failed to delete root dir: ");
    Serial.println(tempPath);
  } 
}
