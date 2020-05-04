JsonDocument printCurrentLevelDirectories(File dir, EthernetClient client, int level) {
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
