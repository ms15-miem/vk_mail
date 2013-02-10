#ifndef GMAIL_H
#define GMAIL_H

#include "oauth.h"

class GMail : public OAuth
{
    Q_OBJECT

protected:
    QString refresh_token;
    int checkIntervalMinutes;
    void requestToken();

public:
    explicit GMail(QObject *parent = 0);
    ~GMail();
    void setCheckInterval(int minutes);
    int getCheckInterval();

    // тестовые функции
    void test();

signals:
    void unreadedMessage(Message msg);

public slots:
    void startCheckCycle();
};

#endif // GMAIL_H
