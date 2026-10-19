#include "Logger/Logger.h"
#include "WiFiAP.h"

#define DHCP_RETRIES 3

WiFiAP::WiFiAP(uint8_t maxClients) {
  this->maxClients = maxClients;
}

bool WiFiAP::init(const char *ssid, const char *password) {
  this->ssid = ssid;
  this->password = password;

  WiFi.AP.begin();

  bool softAPCreated = WiFi.AP.create(
    this->ssid,
    this->password,
    1,
    false,
    this->maxClients
  );

  if (!softAPCreated || !this->_dnsServer.start()) {
    ERROR("Unable to create AP");
    return false;
  }

  bool dhcpStarted = false;

  for (int i = 0; i < DHCP_RETRIES; i += 1) {
    dhcpStarted = WiFi.AP.enableDhcpCaptivePortal();

    if (dhcpStarted) {
      break;
    }

    delay(1000);
  }

  if (!dhcpStarted) {
    ERROR("DHCP Captive portal not enabled");
    return false;
  }

  this->apip = WiFi.softAPIP().toString();
  return true;
}
