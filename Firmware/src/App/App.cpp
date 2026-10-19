#include "App.h"

App::App(
  SDManager &sd,
  Bootstrapper &bootstrapper,
  std::unordered_map<std::string, String> config,
  uint8_t port,
  uint8_t maxClients,
  uint16_t menuX,
  uint16_t menuY,
  uint16_t buttonGap,
  uint16_t buttonPadding
) :
  _sd(sd),
  _bootstrapper(bootstrapper),
  display(config[CONFIG_I2C_DRIVER], config[CONFIG_I2C_SCREEN_WIDTH].toInt(), config[CONFIG_I2C_SCREEN_HEIGHT].toInt()),
  server(port),
  menu(menuX, menuY, buttonGap, buttonPadding),
  wifi(maxClients)
{
  this->selectedScreen = Screen::Logs;
  this->_selectedLayoutIndex = 0;
  this->_firstVisibleIndex = 0;
  this->_maxScreenlines = config[CONFIG_I2C_SCREEN_HEIGHT].toInt() / LINE_HEIGHT - PROTECTED_LINES;
  this->stats[STATS_CAPTIVE] = 0;
  this->stats[STATS_REQUESTS] = 0;
  this->_ready = false;
  this->_displayReady = false;
  this->_config = config;
}

void App::addLog(const String str) {
  if (this->logs.size() >= this->_maxScreenlines) {
    this->logs.erase(this->logs.begin());
  }

  this->logs.push_back(str);
}

void App::handleInput() {
  this->menu.handleInput();
}

void App::drawMenu() {
  this->menu.draw(this->display);
}

void App::clear() {
  this->display.clear();
}

void App::incStat(const std::string &stat) {
  if (this->stats.contains(stat)) {
    this->stats[stat.c_str()] += 1;
  }
}

bool App::saveLayout(String &layout) {
  this->_config["layout"] = layout;

  return this->_sd.saveConfig("/config.txt", this->_config);
}

bool App::isReady() {
  return this->_ready;
}

bool App::isDisplayReady() {
  return this->_displayReady;
}

uint16_t App::width() {
  return this->display.raw().width();
}

uint16_t App::height() {
  return this->display.raw().height();
}
