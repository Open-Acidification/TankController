#include <stdio.h>
#include <stdlib.h>

int CmpFunc(const void* p1, const void* p2) {
  const char* aName = *(const char**)p1;
  const char* bName = *(const char**)p2;

  while (*aName != '\0' && *aName == *bName) {
    ++aName;
    ++bName;
  }

  return (*aName - *bName);
}

JsonDocument PrintCurrentLevelDirectories(File dir, EthernetClient client, int level) {
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/plain; charset=UTF-8"));
  client.println(F("Connection: keep-alive"));  // the connection will be closed after completion of the response
  client.println(F("Keep-Alive: timeout=5, max=1000"));
  client.println(F("X-Content-Type-Options: nosniff"));
  client.println();
  Serial.println(dir.name());

  // add directories to array
  char** files;
  files = malloc(40 * sizeof(char*));
  int fileCounter = 0;
  while (true) {
    File entry = dir.openNextFile();
    Serial.println(entry.name());
    if (!entry) {
      // no more files
      break;
    }
    char* hasLetterS = strchr(entry.name(), 'S');
    if (entry.isDirectory() && !hasLetterS) {  // WILL NOT PRINT SYSTEM~1 AT ROOT LEVEL
      files[fileCounter] = malloc(10 * sizeof(char));
      strcpy(files[fileCounter], entry.name());
      fileCounter++;
    }
    entry.close();
  }

  // sort array
  qsort(files, fileCounter, sizeof(files[0]), CmpFunc);

  // add items in sorted array to json array
  StaticJsonDocument<512> doc;
  JsonArray filesArray = doc.to<JsonArray>();
  for (int i = 0; i < fileCounter; i++) {
    filesArray.add(files[i]);
  }

  // free memory
  for (int i = 0; i < fileCounter; i++) {
    free(files[i]);
  }
  free(files);

  Serial.print(F("freeMemory()="));
  Serial.println(freeMemory());

  serializeJson(doc, client);
  return doc;
}
