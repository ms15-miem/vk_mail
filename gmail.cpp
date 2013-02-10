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
