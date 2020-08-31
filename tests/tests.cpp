#include "catch.hpp"
#include "../ClearBuffer.ino"
// #include "../handleRequest.ino"
#include "../ReadUntilSpace.ino"

TEST_CASE( "Clears buffer completely", "[buffer]" ) {
	int bufferSize = 500;
	char html_request_buffer[bufferSize];	
	ClearBuffer(html_request_buffer, bufferSize);

	int emptyCount = 0;
	for (int i = 0; i < bufferSize; i++) {
		if (html_request_buffer[i] == ' ') {
			emptyCount += 1;
		}
	}

    REQUIRE( emptyCount == 500 );
}

TEST_CASE( "Reads until space with a regular string", "[buffer]" ) {
	int bufferSize = 18;
	char html_request_buffer[] = "one word symbols++";
	int counter = 0;
	std::string result = ReadUntilSpace(html_request_buffer, counter, bufferSize);

	REQUIRE( result == "one" );

	result = ReadUntilSpace(html_request_buffer, counter, bufferSize);

	REQUIRE( result == "word" );

	result = ReadUntilSpace(html_request_buffer, counter, bufferSize);

	REQUIRE( result == "symbols++" );
}

TEST_CASE( "Reads until space with a single word string", "[buffer]" ) {
	int bufferSize = 8;
	char html_request_buffer[] = "longword";
	int counter = 0;
	std::string result = ReadUntilSpace(html_request_buffer, counter, bufferSize);

	REQUIRE( result == "longword" );
}

// TEST_CASE( "Request handles properly", "[request]" ) {
// 	MockClient client;
// 	HandleRequest(client);
// }