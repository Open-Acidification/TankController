DynamicJsonDocument printCurrentLevelDirectories(String dirName, EthernetClient client) {
	File dir = SD.open(dirName);

	DynamicJsonDocument doc(1024);
	char name[6];
	JsonArray filesArray = doc.createNestedArray(dir.name().toCharArray(name, 6));
	while (true) {

		File entry =  dir.openNextFile();
		if (!entry) {
			// no more files
			break;
		}
		char* hasLetterS = strchr(entry.name(), 'S');
		if (entry.isDirectory() && !hasLetterS) { // WILL NOT PRINT SYSTEM~1 AT ROOT LEVEL
			client.println(entry.name());
			DynamicJsonDocument directories = printCurrentLevelDirectories(dirName + "/" + entry.name(), client);
			filesArray.add(directories);
		}
		entry.close();
	}
	serializeJson(doc, client);
	client.println();
	return doc;
}