#ifndef __STILLCONTROLLERTASK_H__
#define __STILLCONTROLLERTASK_H__
#include "Settings.h"
#include "SensorData.h"
#include "DataContext.h"
#include <TaskManagerIO.h>
#include "NotificationHelper.h"
#include <AsyncWebSerial.h>

class ControllerTaskClass : public BaseEvent
{
public:
	ControllerTaskClass(SensorDataClass &sensorData, SettingsClass &settings, DataContextClass &context, AsyncWebSerial &webSerial);
	void exec() override;
	uint32_t timeOfNextCheck() override;

private:
	SensorDataClass &_sensorData;
	SettingsClass &_settings;
	DataContextClass &_context;
	AsyncWebSerial &_webSerial;

	void checkTemperatureLimit();
	
	void handleDehumidifier();
	void handleHumidifier();
	void handleFridge();
};

#endif // __STILLCONTROLLERTASK_H__