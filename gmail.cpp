#include "gmail.h"

#include <iostream>

GMail::GMail(QString _clientSecret, QString _redirectUri, QString _clientId,
             QString _settingsGroup, QObject *parent) :
    OAuth(_clientId, _settingsGroup, parent),
    redirect_uri(_redirectUri), client_secret(_clientSecret)
{
    expiredAcceptTokenTimer = new QTimer(this);
    QObject::connect(expiredAcceptTokenTimer, SIGNAL(timeout()), SLOT(slotGetAccessToken()));
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
    if (refresh_token.isNull() || refresh_token == "") {
        QObject::connect(this, SIGNAL(receivedAuthorizationCode()),
                         SLOT(slotGetRefreshAcceptTokens()));
        getAuthorizationCode();
    }
    else {
        // здесь мб проверка не отменен ли доступ у приложения

        slotGetAccessToken();
    }
}

void GMail::test()
{
    emit setReady(true);
}

GMail::HttpAnswer GMail::jsonParser(QString line)
{
    HttpAnswer ans;

    QStringList lines = line.split('\n');

    // куда без костылей?
    lines[lines.count()-2] += ',';

    for(int i = 1; i < lines.count()-1; i++) {
        QStringList list = lines[i].split('\"');
        if (lines[i].startsWith("  \"access_token\"")) {
            ans.access_token = list[list.size()-2];
        }
        else if (lines[i].startsWith("  \"token_type\"")) {
            ans.token_type = list[list.size()-2];
        }
        else if (lines[i].startsWith("  \"expires_in\"")) {
            ans.expires_in = QString(list[list.size()-1].toStdString().substr(3, list[list.size()-1].size()-3-1).c_str()).toInt();
        }
        else if (lines[i].startsWith("  \"refresh_token\"")) {
            ans.refresh_token = list[list.size()-2];
        }
    }

    return ans;
}


void GMail::slotGetAccessToken()
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

void GMail::getAuthorizationCode()
{
    QUrl url("https://accounts.google.com/o/oauth2/auth?response_type=code&client_id=" + client_id + "&redirect_uri=" + redirect_uri + "&scope=https://mail.google.com/mail/feed/atom/");

    //    webView = new QWebView;

    //    webView->load(url);

    //    QObject::connect(webView, SIGNAL(titleChanged(QString)), SLOT(slotTitleChanged(QString)));

    std::cout << "GMail::getAuthorizationCode() - " << url.toString().toStdString() << std::endl;

    std::string authCode;

    std::cout << "Authorization code = "; std::cin >> authCode;

    authorization_code = authCode.c_str();

    emit receivedAuthorizationCode();

    //    webView->show();
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
    emit unreadedMessage(new Message(QString::fromUtf8("тестовое сообщение")));
}


//void GMail::slotTitleChanged(QString title)
//{
//    if (title.startsWith("Success code=")) {
//        authorization_code = QString(title.toStdString().substr(13).c_str());
//        qDebug() << "gmail: authorization_code = " << authorization_code;

//        emit receivedAuthorizationCode();

//        if (webView) {
//            webView->hide();
//            webView->deleteLater();
//            webView = 0;
//        }
//        QApplication::instance()->processEvents();

//    }
//}

void GMail::slotFinished(QNetworkReply *reply)
{
    QString line = reply->readAll();

    qDebug() << line;

    HttpAnswer ans = jsonParser(line);

    access_token = ans.access_token;
    refresh_token = ans.refresh_token;
    expires_in = ans.expires_in;

    expiredAcceptTokenTimer->start(expires_in*1000);

    qDebug() << "GMail: slotFinished";

    if (!access_token.isNull() && access_token != "") {
        emit setReady(true);
    }

    if (!refresh_token.isNull() && refresh_token != "") {
        saveAuthData();
    }

    reply->deleteLater();
}


GMail::HttpAnswer::HttpAnswer():expires_in(0)
{
}
