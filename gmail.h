#ifndef GMAIL_H
#define GMAIL_H

#include <QObject>

#include "settingssaver.h"

#include <vmime/vmime.hpp>

#ifdef Q_OS_WIN32
#include <vmime/platforms/windows/windowsHandler.hpp>
#endif

#ifdef Q_OS_UNIX
#include <vmime/platforms/posix/posixHandler.hpp>
#endif

class QTimer;
class Message;

class myCertVerifier : public vmime::security::cert::certificateVerifier
{
public:
    void verify(vmime::ref<vmime::security::cert::certificateChain> chain);
};

class GMail : public QObject, public SettingsManager
{
    Q_OBJECT
public:
    explicit GMail(int checkIntervalSec, QObject *parent = 0);
    void connect();
    void setCheckInterval(int msec);
    int getCheckInterval();

private:
    QString login, password;
    int checkIntervalSec;
    QTimer *checkEmailTimer;
    // store of emails
    vmime::ref <vmime::net::store> store;
    void loadAuthData();

protected:
    void loadSettings();

signals:
    void unreadedMessage(Message *msg);
    void connected();

public slots:
    void startCheckCycle();

private slots:
    void readEmails();
};

#endif // GMAIL_H
