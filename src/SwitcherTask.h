// HeaterTask.h
#include <TaskManagerIO.h>
#include <AsyncWebSerial.h>

#ifndef _HEATERTASK_h
#define _HEATERTASK_h

#include "Settings.h"
#include "DataContext.h"


class SwitcherTaskClass : public BaseEvent
{
public:
    SwitcherTaskClass(SettingsClass &settings, DataContextClass &context, AsyncWebSerial &webSerial);
    void exec() override;
    uint32_t timeOfNextCheck() override;
protected:
    SettingsClass& _settingsClass;
    DataContextClass& _context;
    AsyncWebSerial& _webSerial; // Add this line

    double _waitTime = 1000;
    int _lastState = LOW;
private:
    bool minimumTimeReached(unsigned long stateChange, int timeout);
};

#endif
