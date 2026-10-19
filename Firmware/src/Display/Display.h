#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SH110X.h>
#include <vector>
#include <Wire.h>

#pragma once

#define DEFAULT_TEXT_SIZE 1

struct TextBounds {
  int16_t x1;
  int16_t y1;
  uint16_t w;
  uint16_t h;
};

class Display {
  public:
    enum class Driver {
      SSD1306,
      SH1107,
      SH1106G
    };

    Display(String driver, const uint16_t screenWidth, const uint16_t screenHeight, const uint8_t textSize = DEFAULT_TEXT_SIZE);
    bool init(uint8_t i2c_sda_pin, uint8_t i2c_scl_pin);
    void clear();

    void drawText(const uint16_t x, const uint16_t y, String text, bool inverted = false);
    void drawBorder(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t size = 1, bool rounded = true, bool fill = false);

    void render();
    TextBounds getBounds(const String &text);

    Adafruit_GFX &raw();
  
  private:
    Adafruit_SSD1306 _ssd1306;
    Adafruit_SH1107 _sh1107;
    Adafruit_SH1106G _sh1106g;
    Adafruit_GFX *_display = nullptr;

    uint8_t _textSize;
    Driver _driver;
    bool _initialized;
};
