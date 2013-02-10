#include "gmail.h"

GMail::GMail(QObject *parent) :
    OAuth(parent)
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
}


void GMail::startCheckCycle()
{
    emit unreadedMessage(Message(QString::fromUtf8("тестовое сообщение")));
}
