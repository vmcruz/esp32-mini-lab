#include "Button.h"
#include "Display/Display.h"
#include "Logger/Logger.h"

Button::Button(uint16_t x, uint16_t y, String label, std::function<void ()> onPress, uint16_t padding) {
  this->_x = x;
  this->_y = y;
  this->label = label;
  this->_pressed = false;
  this->_padding = padding;
  this->_onPress = onPress;
  this->_visible = true;
  this->_isControlled = false;
}

void Button::controlled(uint8_t pin, uint8_t mode) {
  this->_pin = pin;
  this->_mode = mode;
  this->_isControlled = true;

  pinMode(pin, mode);
}

void Button::draw(Display &display, bool selected, uint16_t offsetX, uint16_t offsetY) const {
  ButtonBounds bounds = this->bounds(display);

  display.drawBorder(this->_x + offsetX, this->_y + offsetY, bounds.w, bounds.h, 1, true, selected);
  display.drawText(this->_x + offsetX + this->_padding, this->_y + offsetY + this->_padding, this->label, selected);
}

ButtonBounds Button::bounds(Display &display) const {
  TextBounds bounds = display.getBounds(this->label);
  ButtonBounds btnBounds;

  btnBounds.x = this->_x;
  btnBounds.y = this->_y;
  btnBounds.w = bounds.w + this->_padding * 2;
  btnBounds.h = bounds.h + this->_padding * 2;

  return btnBounds;
}

void Button::onPress(std::function<void ()> onPress) {
  this->_onPress = onPress;
}

bool Button::isPressed() {
  if (this->_mode == INPUT_PULLUP) {
    return digitalRead(this->_pin) == LOW;
  }

  if (this->_mode == INPUT) {
    return digitalRead(this->_pin) == HIGH;
  }

  return false;
}

void Button::listen() {
  if (!this->_isControlled) {
    ERROR("listen can only be called on controlled buttons");
    return;
  }

  bool isPressing = this->isPressed();

  if (isPressing && !this->_pressed) {
    this->_pressed = true;

    this->_onPress();
  }

  if (!isPressing) {
    this->_pressed = false;
  }
}

void Button::press() {
  this->_onPress();
}

void Button::setVisibility(bool visible) {
  this->_visible = visible;
}

bool Button::isVisible() const {
  return this->_visible;
}
