#ifndef OAUTH_H
#define OAUTH_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QWebView>
#include <QSettings>
#include <QNetworkReply>
#include <QDebug>
#include "message.h"
#include <QApplication>
#include <QTextCodec>

class OAuth : public QObject
{
    Q_OBJECT

protected:
    QWebView *webView;
    QString access_token;
    QNetworkAccessManager *netManager;
    bool keepAuth;
    const QString settingsGroup;
    QString client_id;
    QSettings *cfg;

    virtual void saveAuthData() const;
    virtual void loadAuthData();

public:
    explicit OAuth(QString _clientId, QString _settingsGroup, QObject *parent = 0);
    virtual ~OAuth()=0;
    virtual void connect();
    void setKeepAuth(bool keep);
    bool getKeepAuth();
    bool getReady();

signals:
    void receivedAccessToken();
    void setReady(bool ready);
    
protected slots:
    virtual void slotFinished(QNetworkReply *reply);
    virtual void slotGetAccessToken()=0;
};



#endif // OAUTH_H
