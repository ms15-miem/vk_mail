#ifndef OAUTH_H
#define OAUTH_H

#include <QObject>

#include "settingssaver.h"

class QNetworkAccessManager;

class OAuth : public QObject, public SettingsManager
{
    Q_OBJECT
private:
    bool keepAuth;

protected:
    QString access_token;
    QNetworkAccessManager *netManager;
    QString client_id;
    virtual void loadAuthData()=0;
    void loadSettings();

public:
    explicit OAuth(QString _clientId, QString _settingsGroup, bool keepAuth, QObject *parent = 0);
    virtual ~OAuth();

signals:
    void connected();
};



#endif // OAUTH_H
