String readUntilSpace() {
	String result;
	int start = requestCharCounter;
	char c = htmlRequestBuffer[requestCharCounter];
	while (c == ' ' || c == '\n' || c == '\r') {
		c = htmlRequestBuffer[requestCharCounter++];
	}
	while (c != ' ' && c != '\n' && c != '\r') {
		c = htmlRequestBuffer[requestCharCounter++];
	}
	for (int i = start; i < requestCharCounter; i++) {
		result.concat(htmlRequestBuffer[i]);
	}
	return result;
}