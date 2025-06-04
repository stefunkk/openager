#include "WifiServer.h"

WifiServerClass::WifiServerClass(AsyncWebServer *server, SettingsClass &wifiSettings,
                                 SensorDataClass &sensorData, DataContextClass &context, ConfigurationServiceClass &configurationService) 
                                 : _server(*server), _settings(wifiSettings), _sensorData(sensorData), _context(context), _configurationService(configurationService)
{

  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), F("*"));
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), F("content-type"));
}

void WifiServerClass::connectToWifi()
{
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(_settings.wifiSsid.c_str(), _settings.wifiPassword.c_str());

  _server.begin();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  _context.ipAddress = WiFi.localIP().toString();

  Serial.print("IP address: ");
  Serial.print(_context.ipAddress);

  configurePages();
  configureInputs();
}

void WifiServerClass::setupAccessPoint()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  IPAddress localIp(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.softAPConfig(localIp, gateway, subnet);
  WiFi.softAP("OpenAger");

  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  _server.begin();

  configureAPPages();
  configureInputs();
}

void WifiServerClass::configureAPPages()
{
  _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    const char index_html[] PROGMEM = "<html> <head> <meta charset=\"UTF-8\"> </head> <body> <form v-if=\"false\" action=\"/set\" method=\"get\"> <label for=\"wifiSsid\">Wifi SSID</label> <input type=\"text\" name=\"wifiSsid\"><br /><br /> <label for=\"wifiPassword\">Wifi Pass</label> <input type=\"text\" name=\"wifiPassword\"><br /><br /> <input type=\"submit\" value=\"Ok\"> </form> </body> </html>";
    request->send_P(200, "text/html", index_html);
  });
}

void WifiServerClass::configurePages()
{
  _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });

  _server.on("/csv", HTTP_GET, [](AsyncWebServerRequest *request) {
    // auto response = request->beginResponse(200);
    // response->addHeader("Content-Disposition","attachment;filename=data.csv");
    request->send(SPIFFS, "/data.csv", "text/csv", false);
  });

  _server.on("/data", HTTP_GET, [this](AsyncWebServerRequest *request) {
    char data[1000];
    sprintf(data, "{ "
      "\"temp\": %.02f, \"humidity\": %.02f, "
      "\"fridgeState\": %i, \"dehumidifierState\": %i, \"humidifierState\": %i "
      "}",
    _sensorData.temperature, _sensorData.humidity, 
    _settings.fridgeState, _settings.dehumidifierState, _settings.humidifierState);

    request->send(200, "application/json", data);
  });

}

void WifiServerClass::configureInputs()
{
  _server.on("/clearCsv", HTTP_GET, [this](AsyncWebServerRequest *request) {
    _context.clearCsv = true;
    request->send(200, "text/html");
  });

  _server.on("/notificationTest", HTTP_GET, [this](AsyncWebServerRequest *request) {
    NotificationHelperClass::addNotification(_context, "OpenStill", "Testowe powiadomienie");
    request->send(200, "text/html");
  });

  _server.on("/otherConfiguration", HTTP_GET, [this](AsyncWebServerRequest *request) {
    char data[1000];
    sprintf(data, "{ "
      "\"fridgeMinCycleInSeconds\": %i, \"fridgeMinPauseInSeconds\": %i, "
      "\"fridgeOnTemperatureInCelcius\": %.02f, \"fridgeOffTemperatureInCelcius\": %.02f, "
      "\"humidifierOnPercentage\": %.02f, \"humidifierOffPercentage\": %.02f, "
      "\"dehumidifierOnPercentage\": %.02f, \"dehumidifierOffPercentage\": %.02f, \"csvTimeFrameInSeconds\": %i"
                  "}",
      _settings.fridgeMinCycleInSeconds, _settings.fridgeMinPauseInSeconds,
      _settings.fridgeOnTemperatureInCelcius, _settings.fridgeOffTemperatureInCelcius,
      _settings.humidifierOnPercentage, _settings.humidifierOffPercentage,
      _settings.dehumidifierOnPercentage, _settings.dehumidifierOffPercentage, _settings.csvTimeFrameInSeconds);
    request->send(200, "application/json", data);
  });

  
  _server.on("/set", HTTP_GET, [this](AsyncWebServerRequest *request) {
    
    if (request->hasParam(_fridgeState))
    {
      int state = request->getParam(_fridgeState)->value().toInt();
      _settings.fridgeState = state > 0 ? HIGH : LOW;
      _settings.fridgeStateChanged = true;
    }
    if (request->hasParam(_humidifierState))
    {
      int state = request->getParam(_humidifierState)->value().toInt();
      _settings.humidifierState = state > 0 ? HIGH : LOW;
      _settings.humidifierStateChanged = true;
    }
    
    if (request->hasParam(_dehumidifierState))
    {
      int state = request->getParam(_dehumidifierState)->value().toInt();
      _settings.dehumidifierState = state > 0 ? HIGH : LOW;
      _settings.dehumidifierStateChanged = true;
    }
    
    if (request->hasParam(_csvTimeFrameInSeconds))
    {
      int csvTimeFrame = request->getParam(_csvTimeFrameInSeconds)->value().toInt();
      _settings.csvTimeFrameInSeconds = csvTimeFrame;
    }

    if (request->hasParam(_pushNotificationCode))
    {
      String notificationCode = request->getParam(_pushNotificationCode)->value();
      strlcpy(_settings.pushNotificationCode, notificationCode.c_str(), sizeof(_settings.pushNotificationCode));
    }

    if (request->hasParam(_wifiSsid))
    {
      auto wifiSsid = request->getParam(_wifiSsid)->value();
      _settings.wifiSsid = wifiSsid;
    }

    if (request->hasParam(_wifiPassword))
    {
      auto wifiPassword = request->getParam(_wifiPassword)->value();
      _settings.wifiPassword = wifiPassword;
    }

    if (request->hasParam(_fridgeMinCycleInSeconds))
    {
      int minCycle = request->getParam(_fridgeMinCycleInSeconds)->value().toInt();
      _settings.fridgeMinCycleInSeconds = minCycle;
    }

    if (request->hasParam(_fridgeMinPauseInSeconds))
    {
      int minPause = request->getParam(_fridgeMinPauseInSeconds)->value().toInt();
      _settings.fridgeMinPauseInSeconds = minPause;
    }

    if (request->hasParam(_fridgeOnTemperatureInCelcius))
    {
      float onTemp = request->getParam(_fridgeOnTemperatureInCelcius)->value().toFloat();
      _settings.fridgeOnTemperatureInCelcius = onTemp;
    }

    if (request->hasParam(_fridgeOffTemperatureInCelcius))
    {
      float offTemp = request->getParam(_fridgeOffTemperatureInCelcius)->value().toFloat();
      _settings.fridgeOffTemperatureInCelcius = offTemp;
    }

    if (request->hasParam(_humidifierOffPercentage))
    {
      float humidifierOff = request->getParam(_humidifierOffPercentage)->value().toFloat();
      _settings.humidifierOffPercentage = humidifierOff;
    }

    if (request->hasParam(_humidifierOnPercentage))
    {
      float humidifierOn = request->getParam(_humidifierOnPercentage)->value().toFloat();
      _settings.humidifierOnPercentage = humidifierOn;
    }

    if (request->hasParam(_dehumidifierOnPercentage))
    {
      float dehumidifierOn = request->getParam(_dehumidifierOnPercentage)->value().toFloat();
      _settings.dehumidifierOnPercentage = dehumidifierOn;
    }

    if (request->hasParam(_dehumidifierOffPercentage))
    {
      float dehumidifierOff = request->getParam(_dehumidifierOffPercentage)->value().toFloat();
      _settings.dehumidifierOffPercentage = dehumidifierOff;
    }
    
    _configurationService.saveConfiguration();

    request->send(200, "text/html");
    return;
  });
}