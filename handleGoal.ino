void handleGoal(String endpoint, EthernetClient client) {	
	printHeader(client, 200);

	client.println("ph values: ");
	for (int i = 0; i < phSeriesSize; i++) {
		long phValueCurrent = readLineFromSD("pv.txt", i, goalRecordLength);
		client.println(phValueCurrent);
	}

	client.println("ph times: ");
	for (int i = 0; i < phSeriesSize; i++) {
		long phTimeCurrent = readLineFromSD("pt.txt", i, goalRecordLength);
		client.println(phTimeCurrent);
	}

	client.println("ph interval: ");
	client.println(phInterval);

	client.println("ph delay: ");
	client.println(phDelay);

	client.println("temp values: ");
	for (int i = 0; i < tempSeriesSize; i++) {
		long tempValueCurrent = readLineFromSD("tv.txt", i, goalRecordLength);
		client.println(tempValueCurrent);
	}

	client.println("temp times: ");
	for (int i = 0; i < tempSeriesSize; i++) {
		long tempTimeCurrent = readLineFromSD("tt.txt", i, goalRecordLength);
		client.println(tempTimeCurrent);
	}

	client.println("temp interval: ");
	client.println(tempInterval);

	client.println("temp delay: ");
	client.println(tempDelay);
}