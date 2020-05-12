JsonDocument printCurrentLevelDirectories(File dir, EthernetClient client, int level) {
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/plain; charset=UTF-8");
	client.println("Connection: keep-alive");  // the connection will be closed after completion of the response
	client.println("Keep-Alive: timeout=5, max=1000");
	client.println("X-Content-Type-Options: nosniff");
	client.println();
	StaticJsonDocument<512> doc;
	JsonArray filesArray = doc.to<JsonArray>();;
	Serial.println(dir.name());
	while (true) {

		File entry =  dir.openNextFile();
		Serial.println(entry.name());
		if (!entry) {
			// no more files
			break;
		}
		char* hasLetterS = strchr(entry.name(), 'S');
		if (entry.isDirectory() && !hasLetterS) { // WILL NOT PRINT SYSTEM~1 AT ROOT LEVEL
			filesArray.add(entry.name());
		}
		entry.close();
	}
	serializeJson(doc, client);
	return doc;
}
