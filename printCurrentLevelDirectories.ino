void printCurrentLevelDirectories(String dirName, EthernetClient client) {
	File dir = SD.open(dirName);

	DynamicJsonDocument doc(1024);
	JsonObject root = doc.createNestedObject();
	JsonArray coords = root.createNestedArray("files");
	coords.add(48.7507371);
	coords.add(2.2625587);
	serializeJson(doc, client);
	while (true) {

		File entry =  dir.openNextFile();
		if (!entry) {
		// no more files
		break;
		}
		char* hasLetterS = strchr(entry.name(), 'S');
		if (entry.isDirectory() && !hasLetterS) { // WILL NOT PRINT SYSTEM~1 AT ROOT LEVEL
			client.println(entry.name());
		}
		entry.close();
	}
}