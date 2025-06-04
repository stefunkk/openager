#ifndef __CONFIGURATIONSERVICE_H__
#define __CONFIGURATIONSERVICE_H__
#include "DataContext.h"
#include "Settings.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <SPI.h>
#include "FileService.h"

class ConfigurationServiceClass
{
public:
    ConfigurationServiceClass(FileServiceClass &fileService, DataContextClass &context, SettingsClass &settings);
    void loadConfiguration();
    void saveConfiguration();

private:
    DataContextClass &_context;
    SettingsClass &_settings;
    String _fileName = "/config.json";
    FileServiceClass &_fileService;
};

#endif // __CONFIGURATIONSERVICE_H__