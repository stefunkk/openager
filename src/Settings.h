#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <Arduino.h>

class SettingsClass
{
public:
    String wifiSsid = "mrowisko";
    String wifiPassword = "grarzyna69";

    int fridgeRelayPin = 0;
    int humidifierRelayPin = 0;
    int dehumidifierRelayPin = 0;
    int fanInsidePin = 0;
    int fanOutsidePin = 0;
    int uvLightPin = 0;

    float fridgeOnTemperatureInCelcius = 0;
    float fridgeOffTemperatureInCelcius = 0;

    float humidifierOnPercentage = 0;
	float humidifierOffPercentage = 0;
	float dehumidifierOnPercentage = 0;
	float dehumidifierOffPercentage = 0;

    bool fridgeState = LOW;
    bool fridgeStateChanged = false;
    int fridgeMinCycleInSeconds = 240;
    int fridgeMinPauseInSeconds = 360;

    bool dehumidifierState = LOW;
    bool dehumidifierStateChanged = false;
    
    bool humidifierState = LOW;
    bool humidifierStateChanged = false;

    bool uvLightState = LOW;
    bool uvLightStateChanged = false;

    int csvTimeFrameInSeconds = 0;
    
    char pushNotificationCode[100]= "";

    bool saveCsv = false;

    bool fanInsideState = LOW;
    bool fanInsideStateChanged = false;
    
    bool fanOutsideState = LOW;
    bool fanOutsideStateChanged = false;
};

#endif // __SETTINGS_H__