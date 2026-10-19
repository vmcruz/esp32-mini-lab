#include "App.h"

bool App::_prepareData() {
  // load layouts
  this->layouts = this->_sd.list("/layouts", SD_LS_DIRECTORY_MODE);

  if (this->layouts.size() == 0) {
    this->addLog("[ER] No layouts");
    return false;
  }

  if (!this->_config.contains(CONFIG_SSID)) {
    this->addLog("[ER] ssid config");
    return false;
  }

  this->addLog("[OK] SSID " + this->_config[CONFIG_SSID]);

  INFO("SSID: %s", this->_config[CONFIG_SSID].c_str());

  if (!this->_config.contains(CONFIG_LAYOUT)) {
    this->addLog("[ER] layout config");
    return false;
  }

  this->addLog("[OK] Layout " + this->_config[CONFIG_LAYOUT]);
  
  for (int i = 0; i < this->layouts.size(); i += 1) {
    if (this->layouts[i] == this->_config[CONFIG_LAYOUT]) {
      this->_selectedLayoutIndex = i;
      break;
    }
  }

  this->_firstVisibleIndex = this->_selectedLayoutIndex - this->_maxScreenlines / 2;

  if (this->_firstVisibleIndex < 0) {
    this->_firstVisibleIndex = 0;
  }

  const String routesPath = "/layouts/" + this->layouts[this->_selectedLayoutIndex] + "/routes.txt";
  File routes = this->_sd.read(routesPath);

  if (!routes) {
    ERROR("Unable to load %s", routesPath.c_str());
    this->addLog("[ER] Routes!");
    return false;
  }
  
  this->_routes = routes.readString();

  return true;
}
