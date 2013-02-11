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

private:
    QSettings *cfg;
protected:
    QString access_token;
    QNetworkAccessManager *netManager;
    bool keepAuth;
    const QString settingsGroup;
    QWebView *webView;

    virtual void saveAuthData() const;
    virtual void loadAuthData();
    virtual void requestToken()=0;

public:
    explicit OAuth(QString _settingsGroup, QObject *parent = 0);
    virtual ~OAuth()=0;
    void connect();
    void setKeepAuth(bool keep);
    bool getKeepAuth();
    bool getReady();

signals:
    void receivedAccessToken();
    void setReady(bool ready);
    
private slots:
    void slotFinished(QNetworkReply *reply);
};



#endif // OAUTH_H
