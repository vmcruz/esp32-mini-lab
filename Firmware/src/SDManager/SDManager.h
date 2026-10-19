#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include <SD.h>
#include <vector>
#include "StringUtils/StringUtils.h"

#define SD_LS_DIRECTORY_MODE 0
#define SD_LS_FILE_MODE 1

#pragma once

class SDManager {
  public:
    bool init(const uint8_t sd_cs_pin, const uint8_t sd_clk_pin, const uint8_t sd_mosi_pin, const uint8_t sd_miso_pin);
    File open(String path, const char *mode, const bool create = false);
    File read(String path);
    bool write(String path, uint8_t value, const bool create = true);
    bool write(String path, String value, const bool create = true);
    bool append(String path, String value, const bool create = true);
    double size(const int modifier = 1);
    double freeSpace(const int modifier = 1);
    double usedSpace(const int modifier = 1);
    bool exists(String path);
    std::vector<String> list(String path, uint8_t mode = SD_LS_FILE_MODE);

    template <typename T>
    bool saveConfig(String path, std::unordered_map<std::string, T> &configMap);

    template <typename T>
    std::unordered_map<std::string, T> readConfig(String path, std::function<T(const String &)> mapper = [](const String &value) { return value; });

  private:
    uint8_t _sd_cs_pin;
    uint8_t _sd_clk_pin;
    uint8_t _sd_miso_pin;
    uint8_t _sd_mosi_pin;
    bool mounted = false;
};

template <typename T>
bool SDManager::saveConfig(String path, std::unordered_map<std::string, T> &configMap) {
  String serializedConfig = serializeConfig(configMap);

  return this->write(path, serializedConfig);
}

template <typename T>
std::unordered_map<std::string, T> SDManager::readConfig(String path, std::function<T(const String &)> callback) {
  String configData = this->read(path).readString();

  return deserializeConfig(configData, callback);
}
