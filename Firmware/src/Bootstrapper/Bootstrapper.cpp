#include "Bootstrapper.h"
#include "Logger/Logger.h"

bool Bootstrapper::init() {
  if (!LittleFS.begin()) {
    return false;
  }

  File file = LittleFS.open("/bootstrap.txt", FILE_READ);

  if (!file) {
    this->_bootstrapper_bak[CONFIG_SD_CS_PIN] = DEFAULT_SD_CS_PIN;
    this->_bootstrapper_bak[CONFIG_SD_CLK_PIN] = DEFAULT_SD_CLK_PIN;
    this->_bootstrapper_bak[CONFIG_SD_MISO_PIN] = DEFAULT_SD_MISO_PIN;
    this->_bootstrapper_bak[CONFIG_SD_MOSI_PIN] = DEFAULT_SD_MOSI_PIN;

    String serializedBakConfig = serializeConfig(this->_bootstrapper_bak);
    INFO("Fallback Bootstrapper: \n%s", serializedBakConfig.c_str());

    File filew = LittleFS.open("/bootstrap.txt", FILE_WRITE);
    filew.println(serializedBakConfig);

    filew.close();
  } else {
    String data = file.readString(); 

    this->_bootstrapper_bak = deserializeConfig<uint16_t>(data, [](const String &d) { return d.toInt(); });
    INFO("Bootstrapper: \n%s", serializeConfig(this->_bootstrapper_bak).c_str());
  }

  file.close();

  this->sdCSPin = this->_bootstrapper_bak[CONFIG_SD_CS_PIN];
  this->sdCLKPin = this->_bootstrapper_bak[CONFIG_SD_CLK_PIN];
  this->sdMISOPin = this->_bootstrapper_bak[CONFIG_SD_MISO_PIN];
  this->sdMOSIPin= this->_bootstrapper_bak[CONFIG_SD_MOSI_PIN];

  return true;
}

bool Bootstrapper::save(
  uint16_t sdCSPin,
  uint16_t sdCLKPin,
  uint16_t sdMISOPin,
  uint16_t sdMOSIPin
) {
  File fileBak = LittleFS.open("/bootstrap.bak", FILE_WRITE);

  if (!fileBak) {
    return false;
  }

  String serializedBootstrapperBak = serializeConfig(this->_bootstrapper_bak);

  INFO("Saving new bootstrapper bak: \n%s", serializedBootstrapperBak.c_str());
  fileBak.println(serializedBootstrapperBak);
  fileBak.close();

  File file = LittleFS.open("/bootstrap.txt", FILE_WRITE);

  if (!file) {
    return false;
  }

  this->_bootstrapper_bak[CONFIG_SD_CS_PIN] = sdCSPin;
  this->_bootstrapper_bak[CONFIG_SD_CLK_PIN] = sdCLKPin;
  this->_bootstrapper_bak[CONFIG_SD_MISO_PIN] = sdMISOPin;
  this->_bootstrapper_bak[CONFIG_SD_MOSI_PIN] = sdMOSIPin;

  String serializedBootstrapper = serializeConfig(this->_bootstrapper_bak);

  INFO("Saving new bootstrapper: \n%s", serializedBootstrapper.c_str());
  file.println(serializedBootstrapper);
  file.close();

  return true;
}

bool Bootstrapper::restore() {
  if (!LittleFS.exists("/bootstrap.bak")) {
    return false;
  }

  File fileBak = LittleFS.open("/bootstrap.bak", FILE_READ);

  String bakContent = fileBak.readString();

  fileBak.close();

  File file = LittleFS.open("/bootstrap.txt", FILE_WRITE);

  file.println(bakContent);

  file.close();

  return true;
}
