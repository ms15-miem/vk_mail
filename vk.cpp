#include <QtXml/QDomDocument>
#include "vk.h"
#include "vkmessage.h"

Vk::Vk(QString _clientId, QString _settingsGroup, QObject *parent) :
    OAuth(_clientId, _settingsGroup, parent)
{
}

Vk::~Vk()
{
    delete webView;

}

void Vk::connect()
{
    OAuth::connect();

    // здесь мб проверка не отменен ли доступ у приложения

    if (access_token.isNull() || access_token == "") {
        slotGetAccessToken();
    }
    else {
        emit setReady(true);
    }
}

void Vk::slotGetAccessToken()
{
    QUrl url("http://api.vk.com/oauth/authorize?client_id=" + client_id + "&redirect_uri=http://api.vk.com/blank.html&scope=notify,friends,photos,audio,video,docs,notes,pages,wall,groups,messages,ads,offline&display=page&response_type=token");

    webView = new QWebView;

    webView->load(url);

    QObject::connect(webView, SIGNAL(urlChanged(QUrl)), SLOT(slotUrlChanged(QUrl)));

    webView->show();
}

void Vk::slotMessagesRequestFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QString s = QString::fromUtf8(reply->readAll().data());
    qDebug()<<s;
    QDomDocument doc;
    doc.setContent(s);
    qSort(c);
    QDomElement node = doc.documentElement();
    bool ok = true;
    qint32 count = node.firstChild()
            .firstChild()
            .toText()
            .data()
            .toInt(&ok);
    node = node.firstChildElement();
    for(qint32 i = 0; i<count; i++)
    {
        node = node.nextSiblingElement();
        qDebug()<<node.nodeName();
        QString id = node.firstChildElement("from_id").text();
        QString text = node.firstChildElement("text").text();
        QString date = node.firstChildElement("date").text();
        VkMessage *msg = new VkMessage(id,text,date);
        if(id.toInt() >= 0)
        {
            QUrl url_msg("https://api.vkontakte.ru/method/users.get.xml");
            url_msg.addQueryItem("uids", id);
            url_msg.addQueryItem("fields","first_name,last_name,nickname");
            url_msg.addQueryItem("access_token", access_token);
            QNetworkRequest req(url_msg);
            QNetworkReply *rep = netManager->get(req);
            QObject::connect(rep, SIGNAL(finished()), msg, SLOT(slotUserRequestFinished()));
        }
        else
        {
            QUrl url_msg("https://api.vkontakte.ru/method/groups.getById.xml");
            url_msg.addQueryItem("gids", id);
            url_msg.addQueryItem("access_token", access_token);
            QNetworkRequest req(url_msg);
            QNetworkReply *rep = netManager->get(req);
            QObject::connect(rep, SIGNAL(finished()), msg, SLOT(slotGroupRequestFinished()));

        }
    }
}



void Vk::saveAuthData() const
{
    cfg->setValue("access_token", access_token);
}

void Vk::loadAuthData()
{
    access_token = cfg->value("access_token").toString();
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

void Vk::getNewMessages()
{
    if (access_token.isNull())
        return;

    QUrl url_msg("https://api.vkontakte.ru/method/wall.get.xml");
    url_msg.addQueryItem("owner_id", "-49374915");
    url_msg.addQueryItem("access_token", access_token);

    QNetworkRequest req(url_msg);
    QNetworkReply *reply = netManager->get(req);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(slotMessagesRequestFinished()));
    if(reply->isFinished())
    {
        qDebug()<< reply->readAll();
    }
}
