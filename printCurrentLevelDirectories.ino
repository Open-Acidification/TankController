JsonDocument printCurrentLevelDirectories(File dir, EthernetClient client, int level) {
	// File dir = SD.open(dirName);

	// DynamicJsonDocument tempDoc(1024);
	// DynamicJsonDocument nestedDoc(1024);
	// JsonArray tempArray = nestedDoc.createNestedArray("tempArray");
	// tempArray.add(1);
	// tempArray.add(2);
	// tempArray.add(3);
	// JsonArray tempOuterArray = tempDoc.createNestedArray("tempOuterArray");
	// tempOuterArray.add(nestedDoc);
	// serializeJson(tempDoc, client);

	StaticJsonDocument<1024> doc;
	client.println(dir.name());
	client.println("CURRENT LEVEL:");
	client.println(level);
	JsonArray filesArray = doc.createNestedArray(dir.name());
	while (true) {

		File entry =  dir.openNextFile();
		if (!entry) {
			// no more files
			break;
		}
		char* hasLetterS = strchr(entry.name(), 'S');
		if (entry.isDirectory() && !hasLetterS) { // WILL NOT PRINT SYSTEM~1 AT ROOT LEVEL
			if (level < 3) {
				client.println(entry.name());
				// printCurrentLevelDirectories(dirName + "/" + entry.name(), client);
				StaticJsonDocument<1024> directories = printCurrentLevelDirectories(entry, client, level+1);
				client.println("DIRECTORY");
				serializeJson(doc, client);
				serializeJson(directories, client);
				client.println();
				client.println("POST DIRECTORY");
				filesArray.add(directories);
			} else {
				filesArray.add(entry.name());
			}
		}
		entry.close();
	}
	serializeJson(doc, client);
	client.println();
	return doc;
}