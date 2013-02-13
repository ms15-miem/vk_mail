#include "oauth.h"

OAuth::OAuth(QString _clientId, QString _settingsGroup, QObject *parent) :
    QObject(parent), webView(0), keepAuth(false),
    settingsGroup(_settingsGroup), client_id(_clientId)
{
    cfg = new QSettings(QApplication::applicationName()+".ini",QSettings::IniFormat);
    cfg->beginGroup(settingsGroup);

    netManager = new QNetworkAccessManager(this);
    QObject::connect(netManager, SIGNAL(finished(QNetworkReply*)),
                     SLOT(slotFinished(QNetworkReply*)));
}

OAuth::~OAuth()
{
}

void OAuth::connect()
{
    if (keepAuth) {
        loadAuthData();
    }
}

void OAuth::saveAuthData() const
{
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



    qDebug() << "slotFinished" << reply->url();

    reply->deleteLater();
}
