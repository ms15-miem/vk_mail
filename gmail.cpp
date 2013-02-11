#include "gmail.h"

GMail::GMail(QString _settingsGroup, QObject *parent) :
    OAuth(_settingsGroup, parent)
{
}

GMail::~GMail()
{
}

void GMail::setCheckInterval(int minutes)
{
    checkIntervalMinutes = minutes;
}

int GMail::getCheckInterval()
{
    return checkIntervalMinutes;
}

void GMail::test()
{
    emit setReady(true);
}


void GMail::requestToken()
{
//    QUrl url("https://accounts.google.com/o/oauth2/auth");


    test();
}


void GMail::startCheckCycle()
{
    emit unreadedMessage(Message(QString::fromUtf8("тестовое сообщение")));
}
