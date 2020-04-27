DynamicJsonDocument printCurrentLevelDirectories(String dirName, EthernetClient client) {
	File dir = SD.open(dirName);

	DynamicJsonDocument tempDoc(1024);
	DynamicJsonDocument nestedDoc(512);
	JsonArray tempArray = nestedDoc.createNestedArray("tempArray");
	tempArray.add(1);
	tempArray.add(2);
	tempArray.add(3);
	JsonArray tempOuterArray = tempDoc.createNestedArray("tempOuterArray");
	tempOuterArray.add(nestedDoc);
	serializeJson(tempDoc, client);

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
			client.println("DIRECTORY");
			serializeJson(doc, client);
			serializeJson(directories, client);
			client.println("POST DIRECTORY");
			filesArray.add(directories);
		}
		entry.close();
	}
	serializeJson(doc, client);
	client.println();
	return doc;
}