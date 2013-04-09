#ifndef GMAIL_H
#define GMAIL_H

#include "oauth.h"
#include <QTimer>
#include <QStringList>

//class GMail : public OAuth
//{
//    Q_OBJECT

//private:
//    // тестовые функции
//    void test();

//    struct HttpAnswer
//    {
//        QString access_token;
//        QString token_type;
//        int expires_in;
//        QString refresh_token;
//        HttpAnswer();
//    };

//    HttpAnswer jsonParser(QString line);

//protected:
//    QString authorization_code;
//    // в секундах
//    int expires_in;
//    QString refresh_token;
//    int checkIntervalMinutes;
//    QString redirect_uri;
//    QString client_secret;
//    QTimer *expiredAcceptTokenTimer;

//    void saveAuthData() const;
//    void loadAuthData();
//    // здесь пользователь разрешает доступ к приложению
//    void getAuthorizationCode();

//public:
//    explicit GMail(QString _clientSecret, QString _redirectUri, QString _clientId, QString _settingsGroup, QObject *parent = 0);
//    ~GMail();
//    void setCheckInterval(int minutes);
//    int getCheckInterval();
//    void connect();

//signals:
//    void unreadedMessage(Message* msg);
//    void receivedAuthorizationCode();

//protected slots:
//    void slotFinished(QNetworkReply *reply);
////    void slotTitleChanged(QString title);
//    // считается, что authorization code уже получен
//    void slotGetRefreshAcceptTokens();
//    // при условии, что refresh_token уже есть
//    void slotGetAccessToken();

//public slots:
//    void slotStartCheckCycle();
//};

class GMail : public QObject
{
public slots:
    void startCheckCycle();
};

#endif // GMAIL_H
