#include <string.h>

#define FILE_NAME (strrchr(__FILE__, '/') + 1)

#ifndef DEBUG
  #error "DEBUG is not defined"
#endif

#if DEBUG
  #define INFO(format, ...) Serial.printf("[\x1b[1;96mINF\x1b[0m] %-15.15s:%-4d " format "\r\n", FILE_NAME, __LINE__, ##__VA_ARGS__)
  #define ERROR(format, ...) Serial.printf("[\x1b[1;91mERR\x1b[0m] %-15.15s:%-4d " format "\r\n", FILE_NAME, __LINE__, ##__VA_ARGS__)
  #define WARN(format, ...) Serial.printf("[\x1b[1;93mWRN\x1b[0m] %-15.15s:%-4d " format "\r\n", FILE_NAME, __LINE__, ##__VA_ARGS__)
#else
  #define INFO(...)
  #define ERROR(...)
  #define WARN(...)
#endif
