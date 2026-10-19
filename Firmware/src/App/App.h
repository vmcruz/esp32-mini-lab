#include <Arduino.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include "SDManager/SDManager.h"
#include "WiFiAP/WiFiAP.h"
#include "APServer/APServer.h"
#include "Display/Display.h"
#include "Menu/Menu.h"
#include "Logger/Logger.h"
#include "StringUtils/StringUtils.h"
#include "Bootstrapper/Bootstrapper.h"

#pragma once

#define LINE_HEIGHT 8
#define PROTECTED_LINES 2

#define DEFAULT_PIN_UP 17
#define DEFAULT_PIN_DOWN 15
#define DEFAULT_PIN_LEFT 2
#define DEFAULT_PIN_RIGHT 4
#define DEFAULT_PIN_PRESS 16
#define DEFAULT_PIN_MODE INPUT_PULLUP

#define CONFIG_SSID "ssid"

#define CONFIG_LAYOUT "layout"

#define CONFIG_BTN_PIN_UP "btn_pin_up"
#define CONFIG_BTN_PIN_DOWN "btn_pin_down"
#define CONFIG_BTN_PIN_LEFT "btn_pin_left"
#define CONFIG_BTN_PIN_RIGHT "btn_pin_right"
#define CONFIG_BTN_PIN_PRESS "btn_pin_press"
#define CONFIG_BTN_PIN_MODE "btn_pin_mode"

#define CONFIG_I2C_SCREEN_WIDTH "i2c_screen_width"
#define CONFIG_I2C_SCREEN_HEIGHT "i2c_screen_height"
#define CONFIG_I2C_SDA_PIN "i2c_sda_pin"
#define CONFIG_I2C_SCL_PIN "i2c_scl_pin"
#define CONFIG_I2C_DRIVER "i2c_driver"

#define STATS_REQUESTS "requests"
#define STATS_CAPTIVE "captive"

class App {
  public:
    enum class Screen {
      Logs = 0,
      Stats = 1,
      Layouts = 2,
    };

    App(
      SDManager &sd,
      Bootstrapper &bootstrapper,
      std::unordered_map<std::string, String> config,
      uint8_t port,
      uint8_t maxClients,
      uint16_t menuX = 0,
      uint16_t menuY = 0,
      uint16_t buttonGap = 2,
      uint16_t buttonPadding = 2
    );
    void addLog(const String str);
    bool init();
    void handleInput();
    void drawMenu();
    void clear();
    void render(uint16_t padding = 0);
    void renderLogsScreen(uint16_t padding = 0);
    void renderStatsScreen(uint16_t padding = 0);
    void renderLayoutsScreen(uint16_t padding = 0);
    bool saveLayout(String &layout);
    void incStat(const std::string &stat);
    bool isReady();
    bool isDisplayReady();

    std::vector<String> logs;
    std::vector<String> layouts;
    std::unordered_map<std::string, uint16_t> stats;
    Screen selectedScreen;
    uint16_t width();
    uint16_t height();

    Display display;
    Menu menu;
    APServer server;
    WiFiAP wifi;

  private:
    bool _initDisplay(uint16_t i2c_sda_pin, uint16_t i2c_scl_pin);
    void _initMenu();
    bool _initServer();
    bool _initWiFi();
    bool _prepareData();
    uint8_t getLastVisibleLayoutIndex();

    String _routes;
    int16_t _requests;
    int16_t _captive;
    int8_t _selectedLayoutIndex;
    int8_t _firstVisibleIndex;
    uint8_t _maxScreenlines;
    bool _ready;
    bool _displayReady;
    SDManager &_sd;
    Bootstrapper &_bootstrapper;
    std::unordered_map<std::string, String> _config;
};
