#ifndef GMAIL_H
#define GMAIL_H

#include <QObject>
#include <QDebug>

#include <QtGlobal>
#include <iostream>
#include <sstream>

#include <vmime/vmime.hpp>


#ifdef Q_OS_WIN32
#include <vmime/platforms/windows/windowsHandler.hpp>
#endif

#ifdef Q_OS_UNIX
#include <vmime/platforms/posix/posixHandler.hpp>
#endif

using namespace vmime::net;
using namespace std;


class myCertVerifier : public vmime::security::cert::certificateVerifier
{
public:
    void verify(vmime::ref<vmime::security::cert::certificateChain> chain);
};

class GMail : public QObject
{
    Q_OBJECT
public:
    explicit GMail(QString login, QString password, QObject *parent = 0);
    void connect();
    void setCheckInterval(int minutes);
    int getCheckInterval();
    void loadAuthData();

private:
    void readEmails(vmime::ref<store> store);
    QString login, password;
    int checkIntervalMinutes;

signals:
    
public slots:
    void startCheckCycle();
};

#endif // GMAIL_H
