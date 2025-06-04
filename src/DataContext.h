#ifndef __DATACONTEXT_H__
#define __DATACONTEXT_H__
#include <Arduino.h>
#include "StillNotification.h"

class DataContextClass
{
public:
    DataContextClass();

    StillNotificationStruct notifications[10]; 
    
    bool clearCsv = false;
    String foundDeviceString[4] = {};

    unsigned long fridgeOnLastStateChangeTime;
    unsigned long fridgeOffLastStateChangeTime;
    unsigned long humidifierLastStateChangeTime;
    unsigned long dehumidifierLastStateChangeTime;

    String ipAddress = "";
    bool isIpShown = false;

    double flowRate = 0;
};


#endif // __DATACONTEXT_H__