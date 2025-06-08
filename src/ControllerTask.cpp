#include "ControllerTask.h"

ControllerTaskClass::ControllerTaskClass(SensorDataClass &sensorData, SettingsClass &settings, DataContextClass &context,
AsyncWebSerial &webSerial) : _sensorData(sensorData), _settings(settings), _context(context), _webSerial(webSerial)
{
}

void ControllerTaskClass::exec()
{
	checkTemperatureLimit();
}

void ControllerTaskClass::checkTemperatureLimit()
{
	handleFridge();
	handleHumidifier();
	handleDehumidifier();
}

void ControllerTaskClass::handleFridge()
{
	_webSerial.printf("Control temp: %.2f\n", _sensorData.temperature);
	_webSerial.printf("Fridge state: %i\n", _settings.fridgeState);
	_webSerial.printf("On temp: %.2f\n", _settings.fridgeOnTemperatureInCelcius);
	_webSerial.printf("Off temp: %.2f\n", _settings.fridgeOffTemperatureInCelcius);

	if (_sensorData.temperature > _settings.fridgeOnTemperatureInCelcius && _settings.fridgeState == false)
	{
		Serial.println("Temp too high, fridge on.");

		_settings.fridgeState = true;
		_settings.fridgeStateChanged = true;
	}

	if (_sensorData.temperature < _settings.fridgeOffTemperatureInCelcius  && _settings.fridgeState == true)
	{
		Serial.println("Temp too low, fridge off.");

		_settings.fridgeState = false;
		_settings.fridgeStateChanged = true;
	}
}


void ControllerTaskClass::handleHumidifier()
{

	if (_sensorData.humidity <= 0 || _sensorData.temperature <= 0)
	{
		Serial.println("Invalid humidity or temperature value. Skipping.");
		return;
	}

	if (_sensorData.humidity < _settings.humidifierOnPercentage  && _settings.humidifierState == false)
	{
		Serial.println("Humidity too low, humidifer on.");

		_settings.humidifierState = true;
		_settings.humidifierStateChanged = true;
	}

	if (_sensorData.humidity > _settings.humidifierOffPercentage  && _settings.humidifierState == true)
	{
		Serial.println("Humidity too high, humidifer off.");

		_settings.humidifierState = false;
		_settings.humidifierStateChanged = true;
	}
}

void ControllerTaskClass::handleDehumidifier()
{
	if (_sensorData.humidity > _settings.dehumidifierOnPercentage  && _settings.dehumidifierState == false)
	{
		Serial.println("Humidity too high, dehumidifer on.");

		_settings.dehumidifierState = true;
		_settings.dehumidifierStateChanged = true;
	}

	if (_sensorData.humidity < _settings.dehumidifierOffPercentage  && _settings.dehumidifierState == true)
	{
		Serial.println("Humidity too low, dehumidifer off.");

		_settings.dehumidifierState = false;
		_settings.dehumidifierStateChanged = true;
	}
}

uint32_t ControllerTaskClass::timeOfNextCheck()
{
	setTriggered(true);
	return millisToMicros(5000);
}
