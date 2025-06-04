#include "SwitcherTask.h"

SwitcherTaskClass::SwitcherTaskClass(SettingsClass &settingsClass, DataContextClass &context) : _settingsClass(settingsClass), _lastState(LOW), _context(context)
{
	pinMode(_settingsClass.fridgeRelayPin, OUTPUT);
	pinMode(_settingsClass.humidifierRelayPin, OUTPUT);
	pinMode(_settingsClass.dehumidifierRelayPin, OUTPUT);
	pinMode(_settingsClass.fanInsidePin, OUTPUT);
	pinMode(_settingsClass.fanOutsidePin, OUTPUT);
	pinMode(_settingsClass.uvLightPin, OUTPUT);

	digitalWrite(_settingsClass.fridgeRelayPin, HIGH);
	digitalWrite(_settingsClass.humidifierRelayPin, HIGH);
	digitalWrite(_settingsClass.dehumidifierRelayPin, HIGH);
	digitalWrite(_settingsClass.fanInsidePin, HIGH);
	digitalWrite(_settingsClass.fanOutsidePin, HIGH);
	digitalWrite(_settingsClass.uvLightPin, HIGH);
}

uint32_t SwitcherTaskClass::timeOfNextCheck()
{
	setTriggered(true);
	return millisToMicros(_waitTime);
}

void SwitcherTaskClass::exec()
{
	if (_settingsClass.fridgeStateChanged)
	{
		if ((!_settingsClass.fridgeState && !minimumTimeReached( _context.fridgeOnLastStateChangeTime, _settingsClass.fridgeMinCycleInSeconds)) || 
		(_settingsClass.fridgeState && !minimumTimeReached( _context.fridgeOffLastStateChangeTime, _settingsClass.fridgeMinPauseInSeconds)))
		{
			return;
		}

		if (_settingsClass.fridgeState)
		{
			_context.fridgeOnLastStateChangeTime = millis();
		}
		else
		{
			_context.fridgeOffLastStateChangeTime = millis();
		}

		digitalWrite(_settingsClass.fridgeRelayPin, _settingsClass.fridgeState ? LOW : HIGH);
		_settingsClass.fridgeStateChanged = false;
	}

	if (_settingsClass.dehumidifierStateChanged)
	{
		digitalWrite(_settingsClass.dehumidifierRelayPin, _settingsClass.dehumidifierState ? LOW : HIGH);
		_settingsClass.dehumidifierStateChanged = false;
		_context.dehumidifierLastStateChangeTime = millis();

	}

	if (_settingsClass.humidifierStateChanged)
	{
		digitalWrite(_settingsClass.humidifierRelayPin, _settingsClass.humidifierState ? LOW : HIGH);
		_settingsClass.humidifierStateChanged = false;
		_context.humidifierLastStateChangeTime = millis();
	}

	if (_settingsClass.uvLightStateChanged)
	{
		digitalWrite(_settingsClass.uvLightPin, _settingsClass.uvLightState ? LOW : HIGH);
		_settingsClass.uvLightStateChanged = false;
		_context.uvLightLastStateChangeTime = millis();
	}

	if (_settingsClass.fanInsideStateChanged)
	{
		digitalWrite(_settingsClass.fanInsidePin, _settingsClass.fanInsideState ? LOW : HIGH);
		_settingsClass.fanInsideStateChanged = false;
		_context.fanInsideLastStateChangeTime = millis();
	}

	if (_settingsClass.fanOutsideStateChanged)
	{
		digitalWrite(_settingsClass.fanOutsidePin, _settingsClass.fanOutsideState ? LOW : HIGH);
		_settingsClass.fanOutsideStateChanged = false;
		_context.fanOutsideLastStateChangeTime = millis();
	}
}

bool SwitcherTaskClass::minimumTimeReached(unsigned long stateChange, int timeout)
{
	if (millis() - stateChange > (timeout * 1000))
	{
		return true;
	}

	return false;
}

