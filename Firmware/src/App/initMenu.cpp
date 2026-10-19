#include "App.h"

void App::_initMenu() {
  uint8_t pinUp = this->_config.contains(CONFIG_BTN_PIN_UP) ? this->_config[CONFIG_BTN_PIN_UP].toInt() : DEFAULT_PIN_UP;
  uint8_t pinDown = this->_config.contains(CONFIG_BTN_PIN_DOWN) ? this->_config[CONFIG_BTN_PIN_DOWN].toInt() : DEFAULT_PIN_DOWN;
  uint8_t pinLeft = this->_config.contains(CONFIG_BTN_PIN_LEFT) ? this->_config[CONFIG_BTN_PIN_LEFT].toInt() : DEFAULT_PIN_LEFT;
  uint8_t pinRight = this->_config.contains(CONFIG_BTN_PIN_RIGHT) ? this->_config[CONFIG_BTN_PIN_RIGHT].toInt() : DEFAULT_PIN_RIGHT;
  uint8_t pinPress = this->_config.contains(CONFIG_BTN_PIN_PRESS) ? this->_config[CONFIG_BTN_PIN_PRESS].toInt() : DEFAULT_PIN_PRESS;

  uint8_t pinMode = this->_config.contains(CONFIG_BTN_PIN_MODE) ? strtol(this->_config[CONFIG_BTN_PIN_MODE].c_str(), nullptr, 16) : DEFAULT_PIN_MODE;
  
  this->menu.createUIButton("LOGS", this->display, [this]() {
    this->selectedScreen = Screen::Logs;
    this->menu.showButton(2);
    this->menu.hideButton(3);
  });

  this->menu.createUIButton("STATS", this->display, [this]() {
    this->selectedScreen = Screen::Stats;

    this->menu.showButton(2);
    this->menu.hideButton(3);
  });

  this->menu.createUIButton("LAYOUTS", this->display, [this]() {
    this->selectedScreen = Screen::Layouts;

    if (this->layouts.size() > 1) {
      this->menu.hideButton(2);
      this->menu.showButton(3);
    }
  });

  this->menu.createUIButton("SELECT", this->display, [this]() {
    this->menu.press(0);
    
    this->addLog("Saving layout...");
    if (this->saveLayout(this->layouts[this->_selectedLayoutIndex])) {
      this->addLog("Ok!. Needs reset");
    } else {
      this->addLog("Error!");
    }
  });

  this->menu.addInputHandler(pinLeft, pinMode, [this]() {
    this->menu.left();
  });

  this->menu.addInputHandler(pinRight, pinMode, [this]() {
    this->menu.right();
  });

  this->menu.addInputHandler(pinUp, pinMode, [this]() {
    if (this->selectedScreen == Screen::Layouts) {
      this->_selectedLayoutIndex -= 1;
      int8_t size = this->layouts.size();

      if (this->_selectedLayoutIndex <= this->_firstVisibleIndex) {
        this->_firstVisibleIndex = this->_selectedLayoutIndex;
      }

      if (this->_selectedLayoutIndex < 0) {
        this->_selectedLayoutIndex = size - 1;

        this->_firstVisibleIndex = this->_selectedLayoutIndex - this->_maxScreenlines + 1;

        if (this->_firstVisibleIndex < 0) {
          this->_firstVisibleIndex = 0;
        }
      }
    }
  });

  this->menu.addInputHandler(pinDown, pinMode, [this]() {
    if (this->selectedScreen == Screen::Layouts) {
      this->_selectedLayoutIndex += 1;
      int8_t size = this->layouts.size();


      if (this->_selectedLayoutIndex >= this->getLastVisibleLayoutIndex()) {
        this->_firstVisibleIndex += 1;
      }

      if (this->_selectedLayoutIndex >= size ) {
        this->_selectedLayoutIndex = 0;
        this->_firstVisibleIndex = 0;
      }
    }
  });

  this->menu.addInputHandler(pinPress, pinMode, [this]() {
    this->menu.press();
  });
}
