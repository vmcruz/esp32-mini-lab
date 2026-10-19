#include "Display.h"
#include "Logger/Logger.h"

Display::Display(String driver, const uint16_t screenWidth, const uint16_t screenHeight, const uint8_t textSize)
  : _ssd1306(screenWidth, screenHeight),
  _sh1107(screenWidth, screenHeight),
  _sh1106g(screenWidth, screenHeight)
{
  this->_textSize = textSize;
  this->_initialized = false;

  INFO("Selected driver: %s", driver);

  if (driver == "ssd1306") {
    INFO("Setting SSD1306 driver...");
    this->_driver = Driver::SSD1306;
    this->_display = &this->_ssd1306;
  } else if (driver == "sh1106g") {
    INFO("Setting SH1106G driver...");
    this->_driver = Driver::SH1106G;
    this->_display = &this->_sh1106g;
  } else if (driver == "sh1107") {
    INFO("Setting SH1107 driver...");
    this->_driver = Driver::SH1107;
    this->_display = &this->_sh1107;
  }
}

bool Display::init(uint8_t i2c_sda_pin, uint8_t i2c_scl_pin) {
  if (!Wire.begin(i2c_sda_pin, i2c_scl_pin)) {
    ERROR("Wire initialization failed");
    return false;
  }

  this->_initialized = false;

  switch (this->_driver) {
    case Driver::SSD1306:
      INFO("Initializing SSD1306 driver...");
      this->_initialized = this->_ssd1306.begin(SSD1306_SWITCHCAPVCC, 0x3C, true, false);
      break;
    case Driver::SH1106G:
      INFO("Initializing SH1106G driver...");
      this->_initialized = this->_sh1106g.begin(0x3C, true);
      break;
    case Driver::SH1107:
      INFO("Initializing SH1107 driver...");
      this->_initialized = this->_sh1107.begin(0x3C, true);
      break;
  }

  if (!this->_initialized) {
    ERROR("OLED initialization failed");
    return false;
  } else {
    INFO("OLED initialized correctly");
  }

  this->clear();
  INFO("Display cleared");

  this->render();
  INFO("Display rendered");

  return true;
}

Adafruit_GFX &Display::raw() {
  return *this->_display;
}

void Display::clear() {
  switch (this->_driver) {
    case Driver::SSD1306:
      this->_ssd1306.clearDisplay();
      break;
    case Driver::SH1106G:
      this->_sh1106g.clearDisplay();
      break;
    case Driver::SH1107:
      this->_sh1107.clearDisplay();
      break;
  }
}

void Display::render() {
  switch (this->_driver) {
    case Driver::SSD1306:
      this->_ssd1306.display();
      break;
    case Driver::SH1106G:
      this->_sh1106g.display();
      break;
    case Driver::SH1107:
      this->_sh1107.display();
      break;
  }
}

void Display::drawText(const uint16_t x, const uint16_t y, String text, bool inverted) {
  this->_display->setCursor(x, y);

  if (inverted) {
    this->_display->setTextColor(0, 1);
  } else {
    this->_display->setTextColor(1);
  }

  this->_display->write(text.c_str());
}

void Display::drawBorder(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t size, bool rounded, bool fill) {
  this->_display->fillRoundRect(
    x,
    y,
    w,
    h,
    rounded,
    1
  );

  if (!fill) {
    this->_display->fillRoundRect(
      x + size,
      y + size,
      w - size * 2,
      h - size * 2,
      rounded,
      0
    );
  }
}

TextBounds Display::getBounds(const String &text) {
  TextBounds t;

  this->_display->getTextBounds(
    text,
    0,
    0,
    &t.x1,
    &t.y1,
    &t.w,
    &t.h
  );

  return t;
}
