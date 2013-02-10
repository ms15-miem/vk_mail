#include "oauth.h"

OAuth::OAuth(QObject *parent) :
    QObject(parent), /*webView(0),*/ keepAuth(false)
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
    QSettings cfg;
    cfg.beginGroup(settingsGroup);
    cfg.setValue("access_token", access_token);
    cfg.endGroup();
}

void OAuth::loadAuthData()
{
    QSettings cfg;
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

bool OAuth::getReady()
{
    return ready;
}

void OAuth::slotFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();



    qDebug() << "slotFinished" << reply->url();

    reply->deleteLater();
}

void OAuth::slotReady(bool _ready)
{
    ready = _ready;
}
