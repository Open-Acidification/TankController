#include <string>

String ReadUntilSpace(char* buffer, int& end, int size) {
  String result = "";
  int start = end;
  char c = buffer[end];
  while (c == ' ' || c == '\n' || c == '\r') {
    c = buffer[++end];
    start++;
  }
  while (c != ' ' && c != '\n' && c != '\r' && end < size) {
    c = buffer[++end];
  }
  for (int i = start; i < end; i++) {
    result += buffer[i];
  }
  return result;
}
