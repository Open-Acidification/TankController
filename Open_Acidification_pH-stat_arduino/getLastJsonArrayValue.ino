long GetLastJsonArrayValue(JsonArray array) {
  JsonArray::iterator it = array.begin();
  JsonArray::iterator it2 = array.begin();
  for (it; it != array.end();) {
    it++;
    if (it == array.end()) {
      break;
    } else {
      it2++;
    }
  }
  return it2->as<long>();
}