#include "oauth.h"
#include <QSettings>

OAuth::OAuth(QString _settingsGroup, QObject *parent) :
    QObject(parent), /*webView(0),*/ keepAuth(false), settingsGroup(_settingsGroup)
{
    netManager = new QNetworkAccessManager(this);
    QObject::connect(netManager, SIGNAL(finished(QNetworkReply*)),
                     SLOT(slotFinished(QNetworkReply*)));
}

OAuth::~OAuth()
{
}

void OAuth::connect()
{
    loadAuthData();
    if (keepAuth && !access_token.isNull()) {
        //TODO нужна какая-нибудь проверка на истечение токена
        emit setReady(true);
    }
    else {
        requestToken();
    }
}

void OAuth::saveAuthData() const
{
    QSettings cfg("gmail2vk.ini",QSettings::IniFormat);
    cfg.beginGroup(settingsGroup);
    cfg.setValue("access_token", access_token);
    cfg.endGroup();
}

void OAuth::loadAuthData()
{
    QSettings cfg("gmail2vk.ini",QSettings::IniFormat);
    cfg.beginGroup(settingsGroup);
    access_token = cfg.value("access_token").toString();
    cfg.endGroup();
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
    QByteArray data = reply->readAll();



    qDebug() << "slotFinished" << reply->url();

    reply->deleteLater();
}
