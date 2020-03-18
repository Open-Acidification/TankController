void handleRequest(EthernetClient client) {
	Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    int count = 0;
    clearBuffer();
    requestCharCounter = 0;
    while (client.connected()) {
      if (client.available()) {
        // Here is where the POST data is.  
        while(client.available())
        {          
          htmlRequestBuffer[count] = client.read();
          char c = htmlRequestBuffer[count];
          count++;
        }
        break;
      }
    }
    Serial.println();
    Serial.println("REQUEST: ");
    Serial.println(htmlRequestBuffer);
    Serial.println("=======");
    Serial.println("TYPE: ");
    String type = readUntilSpace();
    Serial.println(type);
    Serial.println("=======");
    Serial.println("ENDPOINT: ");
    String endpoint = readUntilSpace();
    Serial.println(endpoint);
    Serial.println("=======");
    Serial.println("PROTOCOL: ");
    String protocol = readUntilSpace();
    Serial.println(protocol);
    Serial.println("=======");
    Serial.println("BODY: ");
    Serial.println(freeMemory());
    char* firstOpenBrace = strstr(htmlRequestBuffer, "{");
    Serial.println(freeMemory());
    // char* body = calloc(500, sizeof(char));
    // if (body) {
    //   Serial.println("body");
    //   Serial.write(body);
    //   Serial.println("\n!!!!!!!!!!!!!!");
    // } else {
    //   Serial.println("DID NOT ALLOCATE MEMORY FOR BODY");
    // }
    // Serial.println(freeMemory());
    // Serial.println("firstOpenBrace");
    // Serial.write(firstOpenBrace);
    // Serial.println("\n!!!!!!!!!!!!!!");
    // Serial.println("body - firstOpenBrace");
    // Serial.println(body - firstOpenBrace);
    // Serial.println(firstOpenBrace - body);
    // Serial.println("\n!!!!!!!!!!!!!!");
    // strncpy(body, firstOpenBrace, body - firstOpenBrace);
    // Serial.println("body");
    // Serial.println(body);
    // Serial.println("freeMemory");
    // Serial.println(freeMemory());
    // free(body);
    // Serial.println("freed!!!");
    // Serial.println(freeMemory());
    // Serial.println("=======");

	// parse and handle request based on endpoint
    if (endpoint.startsWith("/config")) {
      // handleConfig(body, client);
    } else if (endpoint.startsWith("/device")) {  
      handleDevice(endpoint, client);
    } else {
      handleMisc(client);
    }
}