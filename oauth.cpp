#include "oauth.h"

OAuth::OAuth(QString _clientId, QString _settingsGroup, QObject *parent) :
    QObject(parent), SettingsManager(_settingsGroup), keepAuth(false),
    client_id(_clientId)
{
    netManager = new QNetworkAccessManager(this);
//    QObject::connect(netManager, SIGNAL(finished(QNetworkReply*)),
//                    SLOT(slotFinished(QNetworkReply*)));
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

void OAuth::loadAuthData()
{
}

void OAuth::setKeepAuth(bool keep)
{
    keepAuth = keep;
}

bool OAuth::getKeepAuth()
{
    return keepAuth;
}

void OAuth::slotFinished(QNetworkReply *reply)
{
    //    QByteArray data = reply->readAll();



//    qDebug() << "slotFinished" << reply->url();

    reply->deleteLater();
}
