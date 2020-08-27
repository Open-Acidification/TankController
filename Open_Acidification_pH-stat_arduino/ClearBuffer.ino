#include "string.h"

void ClearBuffer(char* htmlRequestBuffer, int size) {
  memset(htmlRequestBuffer, ' ', size);
}
