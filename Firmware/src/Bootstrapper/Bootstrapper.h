#include <LittleFS.h>
#include <unordered_map>
#include <string>
#include "StringUtils/StringUtils.h"

#pragma once

#define DEFAULT_SD_CS_PIN 5
#define DEFAULT_SD_CLK_PIN 18
#define DEFAULT_SD_MISO_PIN 19
#define DEFAULT_SD_MOSI_PIN 23

#define CONFIG_SD_CS_PIN "sd_cs_pin"
#define CONFIG_SD_CLK_PIN "sd_clk_pin"
#define CONFIG_SD_MISO_PIN "sd_miso_pin"
#define CONFIG_SD_MOSI_PIN "sd_mosi_pin"

class Bootstrapper {
  public:
    uint16_t sdCSPin;
    uint16_t sdCLKPin;
    uint16_t sdMISOPin;
    uint16_t sdMOSIPin;

    bool init();
    bool save(
      uint16_t sdCSPin,
      uint16_t sdCLKPin,
      uint16_t sdMISOPin,
      uint16_t sdMOSIPin
    );

    bool restore();

  private:
    std::unordered_map<std::string, uint16_t> _bootstrapper_bak;
};
