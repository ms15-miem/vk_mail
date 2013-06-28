#ifndef OAUTH_H
#define OAUTH_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QNetworkReply>
#include <QDebug>
#include <QApplication>
#include <QTextCodec>
#include "message.h"
#include "settingssaver.h"

class OAuth : public QObject, public SettingsManager
{
    Q_OBJECT

protected:
    QString access_token;
    QNetworkAccessManager *netManager;
    bool keepAuth;
    QString client_id;

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
