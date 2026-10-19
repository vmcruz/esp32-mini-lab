#include <Arduino.h>
#include <vector>
#include <unordered_map>
#include <string>
#include "Logger/Logger.h"
#include "App/App.h"
#include "StringUtils/StringUtils.h"
#include "Bootstrapper/Bootstrapper.h"
#include "SDManager/SDManager.h"

#define PADDING 3
#define BUTTON_GAP 3
#define BUTTON_PADDING 3
#define MAX_CLIENTS 15

Bootstrapper bootstrapper;
SDManager sd;
App *app;

void drawAppBorder() {
  app->display.drawBorder(0, 0, app->width(), app->height() - 4, 1);
}

void setup() {
  Serial.begin(115200);

  if (!bootstrapper.init()) {
    ERROR("Unable to boot");
    return;
  }

  INFO("[BOOT][SD] CS: %d", bootstrapper.sdCSPin);
  INFO("[BOOT][SD] CLK: %d", bootstrapper.sdCLKPin);
  INFO("[BOOT][SD] MISO: %d", bootstrapper.sdMISOPin);
  INFO("[BOOT][SD] MOSI: %d", bootstrapper.sdMOSIPin);

  if (!sd.init(bootstrapper.sdCSPin, bootstrapper.sdCLKPin, bootstrapper.sdMOSIPin, bootstrapper.sdMISOPin)) {
    ERROR("Cannot initialize SD!");
    INFO("Restoring bootstrapper from .bak");

    if (!bootstrapper.restore()) {
      INFO("Restoring bak failed!");
      return;
    } else {
      INFO("Restored!. Please restart.");
    }
    return;
  }

  if (!sd.exists("/config.txt")) {
    ERROR("/config.txt not found in sd");
    return;
  }

  std::unordered_map<std::string, String> config = sd.readConfig<String>("/config.txt");

  uint16_t sdCSPin = config[CONFIG_SD_CS_PIN].toInt();
  uint16_t sdCLKPin = config[CONFIG_SD_CLK_PIN].toInt();
  uint16_t sdMISOPin = config[CONFIG_SD_MISO_PIN].toInt();
  uint16_t sdMOSIPin= config[CONFIG_SD_MOSI_PIN].toInt();

  bool bootstrapperUpdated = false;

  if (
    sdCSPin != bootstrapper.sdCSPin ||
    sdCLKPin != bootstrapper.sdCLKPin ||
    sdMISOPin != bootstrapper.sdMISOPin ||
    sdMOSIPin != bootstrapper.sdMOSIPin
  ) {
    bootstrapperUpdated = bootstrapper.save(
      sdCSPin,
      sdCLKPin,
      sdMISOPin,
      sdMOSIPin
    );

    if (!bootstrapperUpdated) {
      ERROR("Could not update the bootstrap");
    }
  }

  app = new App(sd, bootstrapper, config, 80, MAX_CLIENTS, 4, config[CONFIG_I2C_SCREEN_HEIGHT].toInt() - 12, BUTTON_GAP, BUTTON_PADDING);

  if (!app->init()) {
    ERROR("Could not initialize App");
    return;
  }

  INFO("App initialized!");

  double chipSize = ESP.getFlashChipSize() / (1024 * 1024);

  app->addLog(String(chipSize) + " Mb Flash size");

  app->addLog("[BOOT] Bootstrapped!");

  drawAppBorder();

  app->selectedScreen = App::Screen::Logs;

  app->menu.hideButton(3);

  if (bootstrapperUpdated) {
    app->addLog("[BOOT] Was updated");
    app->addLog("[BOOT] restart!");
    return;
  }

  app->addLog("Initializing...");

  app->render(PADDING);

  app->server.serve();

  delay(5000);

  app->logs.clear();
}

void loop() {
  if (app->isDisplayReady()) {
    app->clear();

    drawAppBorder();

    app->render(PADDING);
  }

  delay(5);
}
