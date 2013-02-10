#ifndef GMAIL_H
#define GMAIL_H

#include "oauth.h"

class GMail : public OAuth
{
    Q_OBJECT

private:
    // тестовые функции
    void test();

protected:
    QString refresh_token;
    int checkIntervalMinutes;
    void requestToken();

public:
    explicit GMail(QString _settingsGroup, QObject *parent = 0);
    ~GMail();
    void setCheckInterval(int minutes);
    int getCheckInterval();

signals:
    void unreadedMessage(Message msg);

public slots:
    void startCheckCycle();
};

#endif // GMAIL_H
