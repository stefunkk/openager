#ifndef __STILLDATATASK_H__
#define __STILLDATATASK_H__

#include "SensorData.h"
#include "Settings.h"
#include "FileService.h"
#include "DataContext.h"
#include <TaskManagerIO.h>

class DataTaskClass: public BaseEvent
{
public:
    DataTaskClass(DataContextClass& context, FileServiceClass& fileService, SensorDataClass& data, SettingsClass& settings);
	void exec() override;
	uint32_t timeOfNextCheck() override;
private:
    SettingsClass &_settings;
    SensorDataClass &_data;
    String _fileName = "/data.csv";
    FileServiceClass& _fileService;
    DataContextClass& _context;
};


#endif // __STILLDATATASK_H__