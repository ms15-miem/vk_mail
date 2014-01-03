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
    QString client_id;

private:
    bool keepAuth;

public:
    explicit OAuth(QString _clientId, QString _settingsGroup, QObject *parent = 0);
    ~OAuth()=0;
    void loadSettings();
    void loadAuthData();
    void setKeepAuth(bool keep);
    bool getKeepAuth();
    bool getReady();

signals:
    void connected();

protected slots:
    void slotFinished(QNetworkReply *reply);
};



#endif // OAUTH_H
