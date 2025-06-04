#include "SensorTask.h"

SensorTaskClass::SensorTaskClass(Adafruit_SHT4x &sensor, DataContextClass &context, SensorDataClass &sensorData) : _sensor(sensor), _context(context), _sensorData(sensorData)
{
	
	if (!_sensor.begin()) {
		Serial.println("Couldn't find SHT4x");
		while (1) delay(10);
	  }
	  Serial.println("Found SHT4x sensor");
}

uint32_t SensorTaskClass::timeOfNextCheck()
{
	setTriggered(true);

	return millisToMicros(1500);
}

void SensorTaskClass::exec()
{
	sensors_event_t humidity, temp;

	_sensor.getEvent(&humidity, &temp);

	_sensorData.temperature = temp.temperature;
	_sensorData.humidity = humidity.relative_humidity;
}

