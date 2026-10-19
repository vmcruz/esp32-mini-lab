#include <Arduino.h>
#include <functional>

#pragma once

struct ButtonBounds {
  int16_t x;
  int16_t y;
  uint16_t w;
  uint16_t h;
};

class Display;

class Button {
  public:
    Button(uint16_t x, uint16_t y, String label, std::function<void ()> onPress = []() {}, uint16_t padding = 2);
    void controlled(uint8_t pin, uint8_t mode);
    void onPress(std::function<void ()> onPress);
    bool isPressed();
    void listen();
    void press();
    void draw(Display &display, bool selected, uint16_t offsetX = 0, uint16_t offsetY = 0) const;
    ButtonBounds bounds(Display &display) const;
    bool isVisible() const;
    void setVisibility(bool visible);
    String label;

  private:
    uint16_t _x;
    uint16_t _y;
    uint16_t _padding;
    uint8_t _pin;
    uint8_t _mode;
    bool _isControlled;
    std::function<void ()> _onPress;
    bool _pressed;
    bool _visible;
};
