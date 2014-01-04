#include "oauth.h"

#include <QDebug>
#include <QNetworkAccessManager>

OAuth::OAuth(QString _clientId, QString _settingsGroup, bool keepAuth, QObject *parent) :
    QObject(parent), SettingsManager(_settingsGroup), keepAuth(keepAuth),
    client_id(_clientId)
{
    netManager = new QNetworkAccessManager(this);
}

OAuth::~OAuth()
{
}

void OAuth::loadSettings()
{
    if (keepAuth) {
        loadAuthData();
    }
}

