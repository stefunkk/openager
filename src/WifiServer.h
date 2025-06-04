// WifiServerTask.h

#ifndef _WIFISERVERTASK_h
#define _WIFISERVERTASK_h

#include <Arduino.h>
#include "Settings.h"
#include "SensorData.h"
#include "DataContext.h"
#include "ConfigurationService.h"
#include "NotificationHelper.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

class WifiServerClass
{
public:
	WifiServerClass(AsyncWebServer *server, SettingsClass &wifiSettings, SensorDataClass &_sensorData,
					DataContextClass &context, ConfigurationServiceClass &configurationService);
	void connectToWifi();
	void setupAccessPoint();


private:
	AsyncWebServer &_server;
	SettingsClass &_settings;
	SensorDataClass &_sensorData;
	DataContextClass &_context;
	ConfigurationServiceClass &_configurationService;

	const char *_fridgeState = "fridgeState";
	const char *_humidifierState = "humidifierState";
	const char *_dehumidifierState = "dehumidifierState";

	const char *_csvTimeFrameInSeconds = "csvTimeFrameInSeconds";
	
	const char *_wifiSsid = "wifiSsid";
	const char *_wifiPassword = "wifiPassword";
	
	const char* _fridgeMinCycleInSeconds = "fridgeMinCycleInSeconds";
	const char* _fridgeMinPauseInSeconds = "fridgeMinPauseInSeconds";
	const char* _fridgeOnTemperatureInCelcius = "fridgeOnTemperatureInCelcius";
	const char* _fridgeOffTemperatureInCelcius = "fridgeOffTemperatureInCelcius";
	const char* _humidifierOffPercentage = "humidifierOffPercentage";
	const char* _humidifierOnPercentage = "humidifierOnPercentage";
	const char* _dehumidifierOnPercentage = "dehumidifierOnPercentage";
	const char* _dehumidifierOffPercentage = "dehumidifierOffPercentage";

	const char *_pushNotificationCode = "pushNotificationCode";




	void configureAPPages();
	void configurePages();
	void configureInputs();
};

#endif
