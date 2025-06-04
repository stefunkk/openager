#ifndef __NOTIFICATIONTASK_H__
#define __NOTIFICATIONTASK_H__

#include "DataContext.h"
#include "Settings.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <TaskManagerIO.h>

class NotificationTaskClass : public BaseEvent
{
public:
    NotificationTaskClass(DataContextClass &context, SettingsClass &settings);
    void exec() override;
    uint32_t timeOfNextCheck() override;

private:
    DataContextClass &_context;
    SettingsClass &_settings;
    void sendNotification(String title, String message);
};
#endif // __NOTIFICATIONTASK_H__