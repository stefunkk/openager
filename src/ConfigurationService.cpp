#include "ConfigurationService.h"

ConfigurationServiceClass::ConfigurationServiceClass(FileServiceClass &fileService, DataContextClass &context, SettingsClass &settings) : _context(context), _settings(settings), _fileService(fileService)
{
}

void ConfigurationServiceClass::loadConfiguration()
{
    auto fileContent = _fileService.openFile(_fileName);

    if (fileContent.length() == 0)
    {
        return;
    }

    StaticJsonDocument<2500> doc;
    DeserializationError error = deserializeJson(doc, fileContent);

    if (error)
    {
        Serial.println(error.c_str());
        Serial.println(F("Failed to read file, using default configuration"));
    }

    _settings.csvTimeFrameInSeconds = doc["csvTimeFrameInSeconds"];

    _settings.fridgeOnTemperatureInCelcius = doc["fridgeOnTemperatureInCelcius"];
    _settings.fridgeOffTemperatureInCelcius = doc["fridgeOffTemperatureInCelcius"];
    _settings.humidifierOnPercentage = doc["humidifierOnPercentage"];
    _settings.humidifierOffPercentage = doc["humidifierOffPercentage"];
    _settings.dehumidifierOnPercentage = doc["dehumidifierOnPercentage"];
    _settings.dehumidifierOffPercentage = doc["dehumidifierOffPercentage"];
    _settings.fridgeMinCycleInSeconds = doc["fridgeMinCycleInSeconds"];
    _settings.fridgeMinPauseInSeconds = doc["fridgeMinPauseInSeconds"];

    _settings.wifiSsid = doc["wifiSsid"].as<String>();
    _settings.wifiPassword = doc["wifiPassword"].as<String>();

    strlcpy(_settings.pushNotificationCode, doc["pushNotificationCode"], sizeof(_settings.pushNotificationCode));
}

void ConfigurationServiceClass::saveConfiguration()
{
    _fileService.removeFile(_fileName);

    StaticJsonDocument<2500> doc;

    doc["csvTimeFrameInSeconds"] = _settings.csvTimeFrameInSeconds;
    doc["pushNotificationCode"] = _settings.pushNotificationCode;

    doc["fridgeOnTemperatureInCelcius"] = _settings.fridgeOnTemperatureInCelcius;
    doc["fridgeOffTemperatureInCelcius"] = _settings.fridgeOffTemperatureInCelcius;
    doc["humidifierOnPercentage"] = _settings.humidifierOnPercentage;
    doc["humidifierOffPercentage"] = _settings.humidifierOffPercentage;
    doc["dehumidifierOnPercentage"] = _settings.dehumidifierOnPercentage;
    doc["dehumidifierOffPercentage"] = _settings.dehumidifierOffPercentage;
    doc["fridgeMinCycleInSeconds"] = _settings.fridgeMinCycleInSeconds;
    doc["fridgeMinPauseInSeconds"] = _settings.fridgeMinPauseInSeconds;

    doc["wifiSsid"] = _settings.wifiSsid;
    doc["wifiPassword"] = _settings.wifiPassword;

    String content;
    if (serializeJson(doc, content) == 0)
    {
        Serial.println(F("Failed to write to file"));
    }
    
    _fileService.saveFile(_fileName, content);
}