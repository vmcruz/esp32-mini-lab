#include <DNSServer.h>
#include <Arduino.h>
#include <WiFi.h>
#include <math.h>

class WiFiAP {
  public:
    WiFiAP(uint8_t maxClients);
    String apip;
    String ssid;
    String password;
    uint8_t maxClients;

    bool init(const char *ssid, const char *password);

  private:
    DNSServer _dnsServer;
};
