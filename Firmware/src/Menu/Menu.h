#include <Arduino.h>
#include <vector>
#include <functional>
#include "Button/Button.h"

class Display;

struct PinHandler {
  std::function<void ()> callback;
  uint8_t pin;
  uint8_t mode;
  bool pressed;
};

class Menu {
  public:
    Menu(uint16_t x, uint16_t y, uint16_t gap = 2, uint16_t padding = 2);
    void createUIButton(String label, Display &display, std::function<void ()> onPress);
    void handleInput();
    void addInputHandler(uint8_t pin, uint8_t mode, std::function<void ()> callback);
    void draw(Display &display);
    int8_t selectedIndex = 0;
    void right();
    void left();
    void press();
    void press(uint8_t index);
    std::vector<Button> buttons;
    void hideButton(uint8_t index);
    void showButton(uint8_t index);

  private:
    uint16_t _x;
    uint16_t _y;
    uint16_t _gap;
    uint16_t _btnPadding;
    std::vector<PinHandler> _pinHandlers;
    void handlePinInput(PinHandler &pinHandler);
    void move(int8_t dx);

    std::vector<Button *> getVisibleButtons();
};
