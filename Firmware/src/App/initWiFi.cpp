#include "App.h"

bool App::_initWiFi() {
  if (!this->wifi.init(this->_config[CONFIG_SSID].c_str(), nullptr)) {
    this->addLog("[ER] WiFiAP");
    ERROR("Unable to initialize WiFiAP");
    return false;
  }

  this->addLog("[OK] IP " + this->wifi.apip);
  INFO("IP: %s", this->wifi.apip);

  return true;
}
