#ifndef GMAIL_H
#define GMAIL_H

#include "oauth.h"
#include <QTimer>

class GMail : public OAuth
{
    Q_OBJECT

private:
    // тестовые функции
    void test();

protected:
    QString authorization_code;
    // в секундах
    int expires_in;
    QString refresh_token;
    int checkIntervalMinutes;
    QString redirect_uri;
    QString client_secret;
    QTimer *expiredAcceptTokenTimer;

    void saveAuthData() const;
    void loadAuthData();
    bool isAuthDataReady() const;
    // здесь пользователь разрешает доступ к приложению
    void getAuthorizationCode();
    // при условии, что refresh_token уже есть
        void getAcceptToken();

public:
    explicit GMail(QString _clientSecret, QString _redirectUri, QString _clientId, QString _settingsGroup, QObject *parent = 0);
    ~GMail();
    void setCheckInterval(int minutes);
    int getCheckInterval();
    void connect();

signals:
    void unreadedMessage(Message msg);
    void receivedAuthorizationCode();

protected slots:
    void slotFinished(QNetworkReply *reply);
    void slotTitleChanged(QString title);
    // считается, что authorization code уже получен
    void slotGetRefreshAcceptTokens();

public slots:
    void slotStartCheckCycle();
};

#endif // GMAIL_H
