#include "StringUtils.h"

String pad(const String &str, const char *c, uint16_t len, bool fromStart) {
  String result;

  if (str.length() < len) {
    for(int i = 0; i <  len - str.length(); i += 1) {
      result += c;
    }
  }

  return result;
}

String padStart(String str, const char *c, uint16_t len) {
  return pad(str, c, len, true) + str;
}

String padEnd(String str, const char *c, uint16_t len) {
  return str + pad(str, c, len, false);
}

std::vector<String> split(String str, const char *delim) {
  int delimLastIndex = 0;
  int length = str.length();

  std::vector<String> result;

  while(delimLastIndex < length) {
    int delimIndex = str.indexOf(delim, delimLastIndex);

    if (delimIndex == -1) {
      delimIndex = length;
    }

    String chunk = str.substring(delimLastIndex, delimIndex);

    result.push_back(chunk);

    delimLastIndex = delimIndex + 1;
  }

  return result;
}
