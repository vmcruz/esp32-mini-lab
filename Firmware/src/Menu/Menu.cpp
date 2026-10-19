#include "Menu.h"
#include "Display/Display.h"
#include "Button/Button.h"

Menu::Menu(uint16_t x, uint16_t y, uint16_t gap, uint16_t btnPadding) { 
  this->_x = x;
  this->_y = y;
  this->_gap = gap;
  this->_btnPadding = btnPadding;
  this->selectedIndex = 0;
}

void Menu::createUIButton(String label, Display &display, std::function<void ()> onPress) {
  Button button(this->_x, this->_y, label, onPress, this->_btnPadding);

  this->buttons.push_back(button);
}

bool isPinTriggered(uint8_t pin, uint8_t mode) {
  if (mode == INPUT_PULLUP) {
    return digitalRead(pin) == LOW;
  }

  if (mode == INPUT) {
    return digitalRead(pin) == HIGH;
  }

  return false;
}

void Menu::handlePinInput(PinHandler &pinHandler) {
  bool pressed = isPinTriggered(pinHandler.pin, pinHandler.mode);

  if (pressed && !pinHandler.pressed) {
    pinHandler.pressed = true;

    pinHandler.callback();
  }

  if (!pressed) {
    pinHandler.pressed = false;
  }
}

void Menu::addInputHandler(uint8_t pin, uint8_t mode, std::function<void ()> callback) {
  pinMode(pin, mode);

  PinHandler newPinHandler;

  newPinHandler.callback = callback;
  newPinHandler.pin = pin;
  newPinHandler.mode = mode;
  newPinHandler.pressed = false;

  this->_pinHandlers.push_back(newPinHandler);
}

std::vector<Button *> Menu::getVisibleButtons() {
  std::vector<Button *> visibleButtons;

  for (auto &button : this->buttons) {
    if (button.isVisible()) {
      visibleButtons.push_back(&button);
    }
  }

  return visibleButtons;
}

void Menu::draw(Display &display) {
  uint16_t boundsW = 0;

  std::vector<Button *> visibleButtons = this->getVisibleButtons();

  for (int i = 0; i < visibleButtons.size(); i += 1) {
    visibleButtons[i]->draw(display, this->selectedIndex == i, boundsW);

    ButtonBounds bounds = visibleButtons[i]->bounds(display);

    boundsW += bounds.w + this->_gap;
  }
}

void Menu::hideButton(uint8_t index) {
  this->buttons[index].setVisibility(false);
}

void Menu::showButton(uint8_t index) {
  this->buttons[index].setVisibility(true);
}

void Menu::handleInput() {
  for (int i = 0; i < this->_pinHandlers.size(); i += 1) {
    PinHandler &pinHandler = this->_pinHandlers[i];
    this->handlePinInput(pinHandler);
  }
}

void Menu::move(int8_t dx) {
  this->selectedIndex += dx;
  std::vector<Button *> visibleButtons = this->getVisibleButtons();
  int8_t size = visibleButtons.size();

  if(this->selectedIndex < 0) {
    this->selectedIndex = size - 1;
  } else if (this->selectedIndex >= size) {
    this->selectedIndex = 0;
  }
}

void Menu::right() {
  this->move(1);
}

void Menu::left() {
  this->move(-1);
}

void Menu::press() {
  std::vector<Button *> visibleButtons = this->getVisibleButtons();

  if (this->selectedIndex >= 0 && this->selectedIndex < visibleButtons.size()) {
    visibleButtons[this->selectedIndex]->press();
  }
}

void Menu::press(uint8_t index) {
  this->selectedIndex = index;

  this->press();
}
