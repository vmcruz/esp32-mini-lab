#include <Arduino.h>
#include <vector>
#include <unordered_map>
#include <string>

#pragma once

String pad(const String &base, const char *c, uint16_t len, bool fromStart = true);

String padStart(String base, const char *c, uint16_t len);

String padEnd(String base, const char *c, uint16_t len);

std::vector<String> split(String base,  const char *delim);

template <typename T>
String serializeConfig(std::unordered_map<std::string, T> &configMap) {
  String serializedConfig = "";

  for (const auto& [key, value] : configMap) {
    serializedConfig += String(key.c_str()) + "=" + String(value) + "\n";
  }

  return serializedConfig;
}

template <typename T>
std::unordered_map<std::string, T> deserializeConfig(String data, std::function<T(const String &)> mapper = [](const String &d) { return d; }) {
  std::unordered_map<std::string, T> configMap;

  data.trim();

  std::vector<String> configLines = split(data, "\n");

  for (int i = 0; i < configLines.size(); i += 1) {
    configLines[i].trim();

    if (configLines[i].length() != 0 && !configLines[i].startsWith(";")) {
      std::vector<String> config = split(configLines[i], "=");

      config[0].trim();
      config[1].trim();

      configMap[config[0].c_str()] = mapper(config[1]);
    }
  }

  return configMap;
}
