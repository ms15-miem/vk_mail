#include "vk.h"

Vk::Vk(int _clientId, QString _settingsGroup, QObject *parent) :
    OAuth(_settingsGroup, parent), clientId(_clientId)
{
}

Vk::~Vk()
{
    delete webView;

}

void Vk::requestToken()
{
    QUrl url("http://api.vk.com/oauth/authorize?client_id=" + QString::number(clientId, 10)+ "&redirect_uri=http://api.vk.com/blank.html&scope=notify,friends,photos,audio,video,docs,notes,pages,wall,groups,messages,ads,offline&display=page&response_type=token");

    webView = new QWebView;

    webView->load(url);

    QObject::connect(webView, SIGNAL(urlChanged(QUrl)), SLOT(slotUrlChanged(QUrl)));

    webView->show();

//    netManager->get(QNetworkRequest(url));
}

void Vk::slotUrlChanged(const QUrl &_url)
{
    QUrl url = _url.toString().replace('#', '?');

    if (url.hasQueryItem("access_token")) {
        access_token = url.queryItemValue("access_token");
        qDebug() << access_token;

        if (webView) {
            webView->hide();
            webView->deleteLater();
            webView = 0;
        }
        QApplication::instance()->processEvents();

        emit receivedAccessToken();
        saveAuthData();
        emit setReady(true);
    }
}

void Vk::slotPost(const Message &msg)
{
    if (access_token.isNull()) return;

    QUrl url_msg("https://api.vkontakte.ru/method/wall.post");
    //    QUrl url_msg("https://api.vkontakte.ru/method/wall.post?owner_id=-49374915&message=check&from_group=1&access_token=" + access_token);

    url_msg.addQueryItem("owner_id", "-49374915");

    QString message = msg.getText();

    url_msg.addEncodedQueryItem("message", QUrl::toPercentEncoding(message));

    url_msg.addQueryItem("from_group", "0");
    //            url_msg.addQueryItem("from_group", "1");
    url_msg.addQueryItem("access_token", access_token);

    QNetworkRequest req(url_msg);
    netManager->get(req);
}
