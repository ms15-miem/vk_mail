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

class OAuth : public QObject
{
    Q_OBJECT

protected:
    QString access_token;
    QNetworkAccessManager *netManager;
    QWebView *webView;
    bool keepAuth;
    QString settingsGroup;

    virtual void saveAuthData() const;
    virtual void loadAuthData();
    virtual void requestToken()=0;

public:
    explicit OAuth(QObject *parent = 0);
    virtual ~OAuth()=0;
    void connect();
    void setKeepAuth(bool keep);
    bool getKeepAuth();

signals:
    void receivedAccessToken();
    
private slots:
    void slotFinished(QNetworkReply *reply);
};



#endif // OAUTH_H
