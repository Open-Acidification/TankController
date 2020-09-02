#include "string.h"

void ClearBuffer(char* html_request_buffer, int size) {
  memset(html_request_buffer, ' ', size);
}
