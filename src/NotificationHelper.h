#ifndef __NOTIFICATIONHELPER_H__
#define __NOTIFICATIONHELPER_H__

#include "DataContext.h"

class NotificationHelperClass
{
public:
    static bool addNotification(DataContextClass &context, String name, String message);
};
#endif // __NOTIFICATIONHELPER_H__