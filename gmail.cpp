#include "gmail.h"

GMail::GMail(QString _clientSecret, QString _redirectUri, QString _clientId,
             QString _settingsGroup, QObject *parent) :
    OAuth(_clientId, _settingsGroup, parent),
    redirect_uri(_redirectUri), client_secret(_clientSecret)
{
    expiredAcceptTokenTimer = new QTimer(this);
    QObject::connect(expiredAcceptTokenTimer, SIGNAL(timeout()), SLOT(getAcceptToken()));
}

GMail::~GMail()
{
}

void GMail::setCheckInterval(int minutes)
{
    checkIntervalMinutes = minutes;
}

int GMail::getCheckInterval()
{
    return checkIntervalMinutes;
}

void GMail::connect()
{
    OAuth::connect();
    if (refresh_token.isNull()) {
        QObject::connect(this, SIGNAL(receivedAuthorizationCode()),
                         SLOT(slotGetRefreshAcceptTokens()));
        getAuthorizationCode();
    }
    else {
        // здесь мб проверка не отменен ли доступ у приложения

        getAcceptToken();
    }
}

void GMail::test()
{
    emit setReady(true);
}


void GMail::getAcceptToken()
{
    QUrl url("https://accounts.google.com/o/oauth2/token");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrl params;
    params.addQueryItem("refresh_token", refresh_token);
    params.addQueryItem("client_id", client_id);
    params.addQueryItem("client_secret", client_secret);
    params.addQueryItem("grant_type", "refresh_token");

    netManager->post(request, params.encodedQuery());
}

void GMail::saveAuthData() const
{
    cfg->setValue("refresh_token", refresh_token);
}

void GMail::loadAuthData()
{
    refresh_token = cfg->value("refresh_token").toString();
}

bool GMail::isAuthDataReady() const
{
    return false;
}

void GMail::getAuthorizationCode()
{
    QUrl url("https://accounts.google.com/o/oauth2/auth?response_type=code&client_id=" + client_id + "&redirect_uri=" + redirect_uri + "&scope=https://mail.google.com/mail/feed/atom/");

    webView = new QWebView;

    webView->load(url);

    QObject::connect(webView, SIGNAL(titleChanged(QString)), SLOT(slotTitleChanged(QString)));

    webView->show();
}

void GMail::slotGetRefreshAcceptTokens()
{
    QUrl url("https://accounts.google.com/o/oauth2/token");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrl params;
    params.addQueryItem("code", authorization_code);
    params.addQueryItem("client_id", client_id);
    params.addQueryItem("client_secret", client_secret);
    params.addQueryItem("redirect_uri", redirect_uri);
    params.addQueryItem("grant_type", "authorization_code");

    netManager->post(request, params.encodedQuery());
}

void GMail::slotStartCheckCycle()
{
    emit unreadedMessage(Message(QString::fromUtf8("тестовое сообщение")));
}


void GMail::slotTitleChanged(QString title)
{
    if (title.startsWith("Success code=")) {
        authorization_code = QString(title.toStdString().substr(13).c_str());
        qDebug() << "gmail: authorization_code = " << authorization_code;

        emit receivedAuthorizationCode();

        if (webView) {
            webView->hide();
            webView->deleteLater();
            webView = 0;
        }
        QApplication::instance()->processEvents();

    }
}

void GMail::slotFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();

    qDebug() << data;

//    if (reply->url().hasQueryItem("refresh_token")) {
//        refresh_token = reply->url().queryItemValue("refresh_token");
//        saveAuthData();
//    }

//    if (reply->url().hasQueryItem("access_token")) {
//        access_token = reply->url().queryItemValue("access_token");
//        emit setReady(true);
//    }

//    if (reply->url().hasQueryItem("expires_in")) {
//        expires_in = reply->url().queryItemValue("expires_in").toInt();
//        expiredAcceptTokenTimer->start(expires_in*1000);
//    }

    qDebug() << "GMail: slotFinished" << reply->url();

    reply->deleteLater();
}
