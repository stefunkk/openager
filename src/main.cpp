#include "ConfigurationService.h"
#include "DataContext.h"
#include "SwitcherTask.h"
#include "Settings.h"
#include "SensorTask.h"
#include <TaskManagerIO.h>
#include <Arduino.h>

#include <WiFi.h>
#include <AsyncTCP.h>

#include <ESPAsyncWebServer.h>
#include "FileService.h"
#include "DataTask.h"
#include "ControllerTask.h"
#include "NotificationTask.h"
#include "WifiServer.h"
#include "FlashDataHelper.h"
#include <Adafruit_SHT4x.h>
#include <Wire.h>

#include <ElegantOTA.h>

#include <AsyncWebSerial.h>
#include <gpio_viewer.h>

const int csvTimeFrameInSeconds = 30;
const bool saveCsv = true;

// // PIN settings
const uint8_t Fridge = GPIO_NUM_26;
const uint8_t Humidifier = GPIO_NUM_27;
const uint8_t Dehumidifier = GPIO_NUM_14;
const uint8_t UvLight = GPIO_NUM_13; // @TODO
const uint8_t FanOutside = GPIO_NUM_25; // @TODO
const uint8_t FanInside = GPIO_NUM_33; // @TODO

const int defaultOperatingTimeInSeconds = 5 * 60;
const int defaultPauseTimeInSeconds = 5 * 60;
const float defaultOnTemperatureInCelcius = 3;
const float defaultOffTemperatureInCelcius = 2;

const float defaultHumidifierOnPercentage = 70;
const float defaultHumidifierOffPercentage = 83;

const float defaultDehumidifierOnPercentage = 85;
const float defaultDehumidifierOffPercentage = 83;

// // WIFI Settings
const String WifiSsid = "";
const String WifiPassword = "";

AsyncWebServer server(80);

Adafruit_SHT4x sht4 = Adafruit_SHT4x();
GPIOViewer gpio_viewer;
AsyncWebSerial webSerial;

void setup()
{
	Serial.begin(115200);
	webSerial.begin(&server);

	ElegantOTA.begin(&server);    // Start ElegantOTA

	 auto *settings = new SettingsClass();

	settings->wifiSsid = WifiSsid;
	settings->wifiPassword = WifiPassword;
	settings->fridgeRelayPin = Fridge;
	settings->humidifierRelayPin = Humidifier;
	settings->dehumidifierRelayPin = Dehumidifier;
	settings->fanInsidePin = FanInside;

	settings->fanInsideState = HIGH; 
	settings->fanInsideStateChanged = true;

	settings->fanOutsidePin = FanOutside;
	settings->uvLightPin = UvLight;

	settings->fridgeMinCycleInSeconds = defaultOperatingTimeInSeconds;
	settings->fridgeMinPauseInSeconds = defaultPauseTimeInSeconds;
	settings->fridgeOnTemperatureInCelcius = defaultOnTemperatureInCelcius;
	settings->fridgeOffTemperatureInCelcius = defaultOffTemperatureInCelcius;

	settings->humidifierOnPercentage = defaultHumidifierOnPercentage;
	settings->humidifierOffPercentage = defaultHumidifierOffPercentage;
	settings->dehumidifierOnPercentage = defaultDehumidifierOnPercentage;
	settings->dehumidifierOffPercentage = defaultDehumidifierOffPercentage;

	settings->csvTimeFrameInSeconds = csvTimeFrameInSeconds;
	settings->saveCsv = saveCsv;

	auto *context = new DataContextClass();

	auto *fileService = new FileServiceClass();

	auto *configurationService = new ConfigurationServiceClass(*fileService, *context, *settings);
	configurationService->loadConfiguration();

	auto *sensorData = new SensorDataClass;
	
	static auto *sensorTask = new SensorTaskClass(sht4, *context, *sensorData);

	static auto *switcherTask = new SwitcherTaskClass(*settings, *context, webSerial);

	auto *dataTask = new DataTaskClass(*context, *fileService, *sensorData, *settings);

	auto *controllerTask = new ControllerTaskClass(*sensorData, *settings, *context, webSerial);

	auto *notificationTask = new NotificationTaskClass(*context, *settings);

	auto *wifiClass = new WifiServerClass(&server, *settings, *sensorData, *context, *configurationService);

	if (settings->wifiSsid == nullptr || settings->wifiSsid == "" || settings->wifiSsid == "null")
	{
		Serial.println("Setting up access point");
		wifiClass->setupAccessPoint();
	}
	else
	{
		wifiClass->connectToWifi();
		
		if (!fileService->fileExists("/index.html"))
		{
			FlashDataHelperClass::downloadContentFiles(*fileService);
		}
	}

	taskManager.registerEvent(sensorTask);

	taskManager.registerEvent(dataTask);
	//taskManager.registerEvent(notificationTask);
	
	taskManager.registerEvent(controllerTask);
	taskManager.registerEvent(switcherTask);


	gpio_viewer.begin();
}

void loop()
{
  taskManager.runLoop();
  ElegantOTA.loop();
  
  delay(100);
}