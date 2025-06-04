#include "DataTask.h"

DataTaskClass::DataTaskClass(DataContextClass &context, FileServiceClass &fileService, SensorDataClass &data, SettingsClass &settings) : _settings(settings), _data(data), _fileService(fileService), _context(context)
{
}

void DataTaskClass::exec()
{
    if (_context.clearCsv)
    {
        _fileService.removeFile(_fileName);
        _context.clearCsv = false;
    }

    char csvEntry[100];
    sprintf(csvEntry, "%i;%.2f;%.2f;%i;%i;%i %\n", (int)(millis()/1000) ,(float)_data.temperature, (float)_data.humidity, _settings.fridgeState, _settings.humidifierState, _settings.dehumidifierState);   
    _fileService.saveFile(_fileName, csvEntry);
}

uint32_t DataTaskClass::timeOfNextCheck()
{
    setTriggered(true);
    return millisToMicros(_settings.csvTimeFrameInSeconds * 1000);
}
