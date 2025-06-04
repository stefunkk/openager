// SensorTask.h
#ifndef _SENSORTASK_h
#define _SENSORTASK_h

#include <TaskManagerIO.h>
#include "DataContext.h"
#include <Adafruit_SHT4x.h>
#include <Wire.h>
#include "SensorData.h"

class SensorTaskClass : public BaseEvent
{
public:
	SensorTaskClass(Adafruit_SHT4x &sensor, DataContextClass& context, SensorDataClass &sensorData);
	void exec() override;
	uint32_t timeOfNextCheck() override;
private:
	DataContextClass& _context;
	Adafruit_SHT4x& _sensor;
	SensorDataClass& _sensorData;
};

#endif
