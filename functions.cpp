#include "functions.h"

#include <QEventLoop>
#include <QTimer>

functions::functions()
{
}

void functions::wait(int msec)
{
    QEventLoop loop;
    QTimer sleepTimer;
    QObject::connect(&sleepTimer, SIGNAL(timeout()), &loop, SLOT(quit()));

    sleepTimer.start(msec);
    loop.exec();
}
