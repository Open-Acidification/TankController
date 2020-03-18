void clearBuffer() {
	for (int i = 0; i < sizeof(htmlRequestBuffer); i++) {
		htmlRequestBuffer[i] = ' ';
	}
}