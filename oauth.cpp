#include "oauth.h"

OAuth::OAuth(QString _settingsGroup, QObject *parent) :
    QObject(parent), /*webView(0),*/ keepAuth(false), settingsGroup(_settingsGroup)
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
    cfg->setValue("access_token", access_token);
}

void OAuth::loadAuthData()
{
    access_token = cfg->value("access_token").toString();
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
