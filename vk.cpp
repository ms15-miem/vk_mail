#include <QtXml/QDomDocument>
#include <QQueue>
#include <QDateTime>
#include <QTimer>
#include "vk.h"

Vk::Vk(QString _clientId, QString _settingsGroup, QObject *parent) :
    OAuth(_clientId, _settingsGroup, parent)
{
    lastId = -1;
    nextLastId = -1;
    lastDate = QDateTime::currentDateTime().toTime_t();
    nextLastDate = 0;
    offset = 0;
    refreshTimer = new QTimer(this);
    checkIntervalMinutes = 1;
    QObject::connect(refreshTimer, SIGNAL(timeout()), this, SLOT(slotGetMessages()));
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

void Vk::setCheckInterval(int minutes)
{
    checkIntervalMinutes = minutes;
}

int Vk::getCheckInterval()
{
    return checkIntervalMinutes;
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
  //  qDebug()<<s;
    QDomDocument doc;
    doc.setContent(s);
    QDomElement node = doc.documentElement().firstChildElement();

    bool ok = true;
    bool fin = false;
    bool first = false;
    qint32 count = node.text().toInt(&ok);
    if(count == 0)
        fin = true;
    if (offset == 0)
        first = true;

    for(qint32 i = 0; i<count; i++)
    {
        node = node.nextSiblingElement();
        QString id = node.firstChildElement("from_id").text();
        QString text = node.firstChildElement("text").text();
        QString date = node.firstChildElement("date").text();
        qint32 msgId = node.firstChildElement("id").text().toInt();
        qint32 intId = id.toInt();
        if(first)
        {
            nextLastId = msgId;
            first = false;
            nextLastDate = date.toInt();
        }
        if (msgId<=lastId || date.toInt() < lastDate)
        {
            fin = true;
            break;
        }
        VkMessage *msg = new VkMessage(id,text,date);
        pendingMessages<<msg;

        if(intId >= 0 && !uidsToRequest.contains(intId))
            uidsToRequest<<intId;
        else if (intId<0 && !gidsToRequest.contains(intId))
            gidsToRequest<<-intId;

    }
    if(fin)
    {
        lastId = nextLastId;
        lastDate = nextLastDate;
        offset = 0;

        requestGroups(gidsToRequest);
        requestUsers(uidsToRequest);
    }
    else
    {
        offset += 100;
        slotGetMessages();
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

void Vk::slotStartCheckCycle()
{
    refreshTimer->start(2000);
//    refreshTimer->start(checkIntervalMinutes*60000);
}

void Vk::slotGetMessages()
{
    if (access_token.isNull())
        return;

    QUrl url_msg("https://api.vkontakte.ru/method/wall.get.xml");
    url_msg.addQueryItem("owner_id", "-49374915");
    if (offset >0)
    {
        url_msg.addQueryItem("offset", QString::number(offset));
    }
    url_msg.addQueryItem("access_token", access_token);

    QNetworkRequest req(url_msg);
    QNetworkReply *reply = netManager->get(req);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(slotMessagesRequestFinished()));
    if(reply->isFinished())
    {
        qDebug()<< reply->readAll();
    }
}


void Vk::requestUsers(QList<qint32> uids)
{
    if (uids.isEmpty())
        return;
    qSort(uids);
    QString uidsString;
    QTextStream stream(&uidsString);
    QList<qint32>::iterator i = uids.begin();
    stream<< *i;
    for(++i; i != uids.end(); ++i)
            stream<<","<<*i;

    QUrl url_msg("https://api.vkontakte.ru/method/users.get.xml");
    url_msg.addQueryItem("uids", uidsString);
    url_msg.addQueryItem("fields","first_name, last_name, nickname");
    url_msg.addQueryItem("access_token", access_token);
    QNetworkRequest req(url_msg);
    QNetworkReply *rep = netManager->get(req);
    QObject::connect(rep, SIGNAL(finished()), this, SLOT(slotUserRequestFinished()));
    uidsToRequest.clear();


}

void Vk::requestGroups(QList<qint32> gids)
{
    if (gids.isEmpty())
        return;
    qSort(gids);
    QString gidsString;
    QTextStream stream(&gidsString);
    QList<qint32>::iterator i = gids.begin();
    stream<< *i;
    for(++i; i != gids.end(); ++i)
            stream<<","<<*i;


    QUrl url_msg("https://api.vkontakte.ru/method/groups.getById.xml");
    url_msg.addQueryItem("gids", gidsString);
    url_msg.addQueryItem("access_token", access_token);
    QNetworkRequest req(url_msg);
    QNetworkReply *rep = netManager->get(req);
    QObject::connect(rep, SIGNAL(finished()), this, SLOT(slotGroupRequestFinished()));
    gidsToRequest.clear();

}

void Vk::slotUserRequestFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QString s = QString::fromUtf8(reply->readAll().data());
    QDomDocument doc;
    doc.setContent(s);
    QDomElement node = doc.documentElement().firstChildElement("user");

    qSort(pendingMessages.begin(), pendingMessages.end(), lessThanById);
    QList<VkMessage*> messages = pendingMessages;
    pendingMessages.clear();
    QList<VkMessage*> messagesToSend;
    QList<VkMessage*>::Iterator msg = messages.begin();
    qint32 id;
    id = node.firstChildElement("uid").text().toInt();
    while(!node.isNull() && msg != messages.end())
    {
        if((*msg)->getId() < id)
        {
            pendingMessages<<*msg;
            ++msg;

        }
        else if((*msg)->getId() == id)
        {
            QString name;
            name += node.firstChildElement("first_name").text() + " ";
            QDomElement nickNode = node.firstChildElement("nickname");
            if(!nickNode.isNull())
                name += nickNode.text() + " ";
            name += node.firstChildElement("last_name").text();
            (*msg)->setAuthor(name);
            messagesToSend<<*msg;
            ++msg;
        }
        else
        {
            node = node.nextSiblingElement("user");
            if (!node.isNull())
                id = node.firstChildElement("uid").text().toInt();

        }
    }

    qSort(messagesToSend.begin(), messagesToSend.end(), lessThanByDate);
    for(msg = messagesToSend.begin(); msg != messagesToSend.end(); ++msg)
    {
        emit unreadedMessage(*(*msg));
        delete (*msg);
    }

}

void Vk::slotGroupRequestFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QString s = QString::fromUtf8(reply->readAll().data());
    QDomDocument doc;
    doc.setContent(s);
    QDomElement node = doc.documentElement().firstChildElement();

    qSort(pendingMessages.begin(), pendingMessages.end(), lessThanById);
    QList<VkMessage*> messages = pendingMessages;
    pendingMessages.clear();
    QList<VkMessage*> messagesToSend;
    QList<VkMessage*>::Iterator msg = messages.begin();
    qint32 id;
    id = node.firstChildElement("gid").text().toInt();
    while(!node.isNull() && msg != messages.end())
    {
        if(-(*msg)->getId() < id)
        {
            pendingMessages<<*msg;
            ++msg;

        }
        else if(-(*msg)->getId() == id)
        {
            (*msg)->setAuthor(node.firstChildElement("name").text());
            messagesToSend<<*msg;
            ++msg;
        }
        else
        {
            node = node.nextSiblingElement("group");
            if (!node.isNull())
                id = node.firstChildElement("gid").text().toInt();

        }
    }

    qSort(messagesToSend.begin(), messagesToSend.end(), lessThanByDate);
    for(msg = messagesToSend.begin(); msg != messagesToSend.end(); ++msg)
    {
        emit unreadedMessage(*(*msg));
        delete *msg;
    }

}
