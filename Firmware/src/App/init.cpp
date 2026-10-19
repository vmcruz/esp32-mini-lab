#include "App.h"

bool App::init() {
  if (!this->_initDisplay(this->_config[CONFIG_I2C_SDA_PIN].toInt(), this->_config[CONFIG_I2C_SCL_PIN].toInt())) {
    return false;
  }

  this->_displayReady = true;
  
  this->_initMenu();
  this->_ready = this->_prepareData() && this->_initWiFi() && this->_initServer();

  return this->_ready;
}
