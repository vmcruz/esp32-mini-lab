#include "App.h"

uint8_t App::getLastVisibleLayoutIndex() {
  uint8_t lastVisibleIndex = this->_firstVisibleIndex + this->_maxScreenlines;

  if (lastVisibleIndex > this->layouts.size()) {
    lastVisibleIndex = this->layouts.size();
  }

  return lastVisibleIndex;
}

void App::renderLogsScreen(uint16_t padding) {
  for (int i = 0; i < this->logs.size(); i += 1) {
    this->display.drawText(padding, LINE_HEIGHT * i + padding, this->logs[i]);
  }
}

void App::renderStatsScreen(uint16_t padding) {
  this->display.drawText(padding, padding + 0, padStart(String(this->stats[STATS_CAPTIVE]), "0", 4) + " Captived");
  this->display.drawText(padding, padding + LINE_HEIGHT, padStart(String(this->stats[STATS_REQUESTS]), "0", 4) + " Requests");
  this->display.drawText(padding, padding + LINE_HEIGHT * 2, "SSID: " + this->wifi.ssid);
  this->display.drawText(padding, padding + LINE_HEIGHT * 3, "APIP: " + this->wifi.apip);
  this->display.drawText(padding, padding + LINE_HEIGHT * 4, "Layouts: " + String(this->layouts.size()));
  this->display.drawText(padding, padding + LINE_HEIGHT * 5, "Routes: " + String(this->server.routes.size()));
}

void App::renderLayoutsScreen(uint16_t padding) {
  uint8_t lastVisibleIndex = this->getLastVisibleLayoutIndex();

  for (int i = this->_firstVisibleIndex; i < lastVisibleIndex; i += 1) {
    bool selected = this->_config[CONFIG_LAYOUT] == this->layouts[i];
    bool highlighted = this->_selectedLayoutIndex == i;

    this->display.drawText(padding, LINE_HEIGHT * (i - this->_firstVisibleIndex) + padding, (selected ? "[*] " : "[ ] ") + this->layouts[i], highlighted);
  }
}

void App::render(uint16_t padding) {
  if (this->selectedScreen == Screen::Logs) {
    this->renderLogsScreen(padding);
  }

  if (this->selectedScreen == Screen::Stats) {
    this->renderStatsScreen(padding);
  }

  if (this->selectedScreen == Screen::Layouts) {
    this->renderLayoutsScreen(padding);
  }

  this->handleInput();

  this->drawMenu();

  this->display.render();
}
