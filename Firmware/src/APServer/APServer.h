#include <Arduino.h>
#include <SD.h>
#include <vector>
#include <functional>
#include <ESPAsyncWebServer.h>

struct Route {
  String route;
  String fileName;
  String mimeType;
};

class APServer {
  public:
    APServer(uint8_t port);
    std::vector<Route> routes;
    
    bool init(String apip, String routes);
    void serve();
    void on(std::function<void(Route route)> callback);
    void route(const String path, std::function<void(AsyncWebServerRequest *request)> callback);

  private:
    String _apip;
    AsyncWebServer _server;
    std::vector<std::function<void(Route route)>> _routeCallbacks;

    void streamSD(AsyncWebServerRequest *request, Route route);
    void handleNotFound(AsyncWebServerRequest *request);
    void handleRoute(AsyncWebServerRequest *request, Route route);
};
