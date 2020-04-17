#include "string.h"

void clearBuffer(char* htmlRequestBuffer, int size) {
	memset(htmlRequestBuffer, ' ', size);
}