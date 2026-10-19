#include "Logger/Logger.h"
#include "APServer.h"
#include "StringUtils/StringUtils.h"

APServer::APServer(uint8_t port) : _server(port) { }

bool APServer::init(String apip, String routes) {
  if (apip.length() == 0) {
    ERROR("AP IP cannot be empty");
    return false;
  }

  routes.trim();

  if (routes.length() == 0) {
    ERROR("Routes cannot be empty");
    return false;
  }

  this->_apip = apip;

  std::vector<String> lines = split(routes, "\n");

  for (int i = 0; i < lines.size(); i += 1) {
    std::vector<String> routeSplit = split(lines[i], ",");
    String route = routeSplit[0];
    String fileName = routeSplit[1];
    String mimeType = routeSplit[2];

    route.trim();
    fileName.trim();
    mimeType.trim();

    Route newRoute;

    newRoute.route = route;
    newRoute.fileName = fileName;
    newRoute.mimeType = mimeType;

    if (route.isEmpty() || fileName.isEmpty() || mimeType.isEmpty()) {
      WARN("Invalid route: %d -> %s", i, lines[i].c_str());
    } else {
      this->routes.push_back(newRoute);
    }
  }

  return true;
}

void APServer::streamSD(AsyncWebServerRequest *request, Route route) {
  INFO("Streaming file: %s (%s)", route.fileName.c_str(), route.mimeType.c_str());

  if (!SD.exists(route.fileName)) {
    ERROR("File doesn't exist: %s", route.fileName.c_str());
    request->send(404, "text/plain", "Not found");
    return;
  }

  request->send(SD, route.fileName, route.mimeType);
}

void APServer::handleNotFound(AsyncWebServerRequest *request) {
  AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
  response->addHeader("Location", "http://" + this->_apip, true);
  request->send(response);
}

void APServer::on(std::function<void(Route route)> callback) {
  this->_routeCallbacks.push_back(callback);
}

void APServer::handleRoute(AsyncWebServerRequest *request, Route route) {
  this->streamSD(request, route);

  for(int i = 0; i < this->_routeCallbacks.size(); i += 1) {
    this->_routeCallbacks[i](route);
  }
}

void APServer::serve() {
  INFO("Loading routes...");

  for (int i = 0; i < this->routes.size(); i += 1) {
    const Route route = this->routes[i];
    
    INFO("route: %s -> %s (%s)", route.route.c_str(), route.fileName.c_str(), route.mimeType.c_str());

    this->_server.on(
      route.route,
      HTTP_GET,
      [this, route](AsyncWebServerRequest *request) {
        this->handleRoute(request, route);
      }
    );
  }

  this->_server.onNotFound([this](AsyncWebServerRequest *request) {
    this->handleNotFound(request);
  });

  this->_server.begin();
}

void APServer::route(const String path, std::function<void(AsyncWebServerRequest *request)> callback) {
  this->_server.on(
    path,
    HTTP_GET,
    callback
  );
}