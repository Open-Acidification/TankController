#include "catch.hpp"
#include "../clearBuffer.ino"
// #include "../handleRequest.ino"
#include "../readUntilSpace.ino"

TEST_CASE( "Clears buffer completely", "[buffer]" ) {
	int bufferSize = 500;
	char htmlRequestBuffer[bufferSize];	
	ClearBuffer(htmlRequestBuffer, bufferSize);

	int emptyCount = 0;
	for (int i = 0; i < bufferSize; i++) {
		if (htmlRequestBuffer[i] == ' ') {
			emptyCount += 1;
		}
	}

    REQUIRE( emptyCount == 500 );
}

TEST_CASE( "Reads until space with a regular string", "[buffer]" ) {
	int bufferSize = 18;
	char htmlRequestBuffer[] = "one word symbols++";
	int counter = 0;
	std::string result = ReadUntilSpace(htmlRequestBuffer, counter, bufferSize);

	REQUIRE( result == "one" );

	result = ReadUntilSpace(htmlRequestBuffer, counter, bufferSize);

	REQUIRE( result == "word" );

	result = ReadUntilSpace(htmlRequestBuffer, counter, bufferSize);

	REQUIRE( result == "symbols++" );
}

TEST_CASE( "Reads until space with a single word string", "[buffer]" ) {
	int bufferSize = 8;
	char htmlRequestBuffer[] = "longword";
	int counter = 0;
	std::string result = ReadUntilSpace(htmlRequestBuffer, counter, bufferSize);

	REQUIRE( result == "longword" );
}

// TEST_CASE( "Request handles properly", "[request]" ) {
// 	MockClient client;
// 	HandleRequest(client);
// }