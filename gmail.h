#ifndef GMAIL_H
#define GMAIL_H

#include <QObject>
#include <QDebug>

#include <QtGlobal>
#include <iostream>
#include <sstream>
#include "settingssaver.h"
#include <QTimer>
#include "gmailmessage.h"
#include <QThread>

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

class GMail : public QObject, public SettingsManager
{
    Q_OBJECT
public:
    explicit GMail(int checkIntervalMsec, QObject *parent = 0);
    void connect();
    void setCheckInterval(int msec);
    int getCheckInterval();

protected:
    void saveAuthData() const;
    void loadAuthData();

private:
    QString login, password;
    int checkIntervalMsec;
    QTimer *checkEmailTimer;
    // store of emails
    vmime::ref <vmime::net::store> store;

signals:
    void unreadedMessage(Message *msg);

public slots:
    void startCheckCycle();

private slots:
    void readEmails();
};

#endif // GMAIL_H
