#include "App.h"

bool App::_initDisplay(uint16_t i2c_sda_pin, uint16_t i2c_scl_pin) {
  if (!this->display.init(i2c_sda_pin, i2c_scl_pin)) {
    return false;
  }

  this->display.raw().setTextWrap(false);
  this->display.raw().setTextSize(1);
  this->display.raw().setTextColor(1);

  return true;
}
