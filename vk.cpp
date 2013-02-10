#include "vk.h"

Vk::Vk(int _clientId, QObject *parent) :
    OAuth(parent), clientId(_clientId)
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

    QObject::connect(webView, SIGNAL(urlChanged(QUrl)), SLOT(slotUrlChanged(QUrl)));

    netManager->get(QNetworkRequest(url));
}

void Vk::slotUrlChanged(const QUrl &_url)
{
    QUrl url = _url.toString().replace('#', '?');

    if (url.hasQueryItem("access_token")) {
        access_token = url.queryItemValue("access_token");
        qDebug() << access_token;

        QObject::disconnect(webView, SIGNAL(urlChanged(QUrl)),
                            this, SLOT(slotUrlChanged(QUrl)));
        webView->hide();
        webView->deleteLater();
        webView = 0;
        QApplication::instance()->processEvents();

        emit receivedAccessToken();
    }
}

void Vk::slotPost(const Message &msg)
{

    //                QUrl msg("https://api.vkontakte.ru/method/wall.post");
    QUrl url_msg("https://api.vkontakte.ru/method/wall.post?owner_id=-49374915&message=check&from_group=1&access_token=" + access_token);


    //        msg.addQueryItem("owner_id", "-49374915");
    //        msg.addQueryItem("message", "test_app");
    //        msg.addQueryItem("from_group", "1");
    //        msg.addQueryItem("access_token", access_token);

    QNetworkRequest req(url_msg);
    netManager->get(req);
}
