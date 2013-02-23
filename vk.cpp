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
    QObject::connect(refreshTimer, SIGNAL(timeout()), this, SLOT(slotCheckMessages()));
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
    qDebug()<<s;
    QDomDocument doc;
    doc.setContent(s);

    QHash<qint32, QString> names = parseUsers(doc).unite(parseGroups(doc));

    bool fin = false;
    QList<VkMessage*> messages = parseMessages(doc, names, &fin);
    pendingMessages.append(messages);
    if(fin)
    {
        lastId = nextLastId;
        lastDate = nextLastDate;
        offset = 0;
        returnMessages();
    }
    else
    {
        offset += 100;
        getMessages();
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

void Vk::slotPost(Message *msg)
{
    if (access_token.isNull()) return;

    QUrl url_msg("https://api.vkontakte.ru/method/wall.post");
    //    QUrl url_msg("https://api.vkontakte.ru/method/wall.post?owner_id=-49374915&message=check&from_group=1&access_token=" + access_token);

    url_msg.addQueryItem("owner_id", "-49374915");

    QString message = msg->getText();
    msg->deleteLater();

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

void Vk::getMessages()
{
    if (access_token.isNull())
        return;

    QUrl url_msg("https://api.vkontakte.ru/method/wall.get.xml");
    url_msg.addQueryItem("owner_id", "-49374915");
    if (offset > 0)
    {
        url_msg.addQueryItem("offset", QString::number(offset));
    }
    url_msg.addQueryItem("extended","1");
    url_msg.addQueryItem("access_token", access_token);

    QNetworkRequest req(url_msg);
    QNetworkReply *reply = netManager->get(req);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(slotMessagesRequestFinished()));
}

void Vk::returnMessages()
{
    qSort(pendingMessages.begin(), pendingMessages.end(), lessThanByDate);
    QList<VkMessage*>::Iterator msg;
    for(msg = pendingMessages.begin(); msg != pendingMessages.end(); ++msg)
    {
        emit unreadedMessage((*msg));
    }
    pendingMessages.clear();
}

void Vk::slotCheckMessages()
{
    if (access_token.isNull())
        return;

    QUrl url_msg("https://api.vkontakte.ru/method/wall.get.xml");
    url_msg.addQueryItem("owner_id", "-49374915");
    url_msg.addQueryItem("count","1");
    url_msg.addQueryItem("access_token", access_token);

    QNetworkRequest req(url_msg);
    QNetworkReply *reply = netManager->get(req);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(slotCheckRequestFinished()));
}

void Vk::slotCheckRequestFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QString s = QString::fromUtf8(reply->readAll().data());
    QDomDocument doc;
    doc.setContent(s);
    QDomElement node = doc.documentElement().firstChildElement("count");

    qint32 count = node.text().toInt();
    if(count == 0)
        return;
    node = node.nextSiblingElement("post");
    QString date = node.firstChildElement("date").text();
    qint32 msgId = node.firstChildElement("id").text().toInt();
    if(msgId > lastId && date.toInt() >= lastDate)
    {
        nextLastId = msgId;
        nextLastDate = date.toInt();
        getMessages();
    }

}



QList<VkMessage *> Vk::parseMessages(QDomDocument &xml, QHash<qint32, QString> &names, bool* fin)
{
    QList<VkMessage *> result;

    QDomElement node = xml.documentElement().firstChildElement("wall");


    node = node.firstChildElement("count");
    qint32 count = node.text().toInt();
    if(count == 0)
    {
        *fin = true;
        return result;
    }

    for(qint32 i = 0; i<count; i++)
    {
        node = node.nextSiblingElement("post");
        QString id = node.firstChildElement("from_id").text();
        QString text = node.firstChildElement("text").text();
        QString date = node.firstChildElement("date").text();
        qint32 msgId = node.firstChildElement("id").text().toInt();
        qint32 intId = id.toInt();
        if (msgId<=lastId || date.toInt() < lastDate)
        {
            *fin = true;
            break;
        }
        VkMessage *msg = new VkMessage(id,text,date);
        if (names.contains(intId))
            msg->setAuthor(names.value(intId));
        msg->addAttachments(parseAttachments(node));
        result<<msg;
    }
    return result;
}

QHash<qint32, QString> Vk::parseUsers(QDomDocument &xml)
{
    QHash<qint32, QString> result;

    QDomElement node = xml.documentElement().firstChildElement("profiles");
    if(node.isNull())
        return result;

    node = node.firstChildElement("user");
    while(!node.isNull())
    {
        qint32 id = node.firstChildElement("uid").text().toInt();
        QString name;
        name += node.firstChildElement("first_name").text() + " ";
        name += node.firstChildElement("last_name").text();
        result.insert(id, name);
        node = node.nextSiblingElement("user");
    }
    return result;
}

QHash<qint32, QString> Vk::parseGroups(QDomDocument &xml)
{
    QHash<qint32, QString> result;

    QDomElement node = xml.documentElement().firstChildElement("groups");
    if(node.isNull())
        return result;

    node = node.firstChildElement("group");
    while(!node.isNull())
    {
        qint32 id = node.firstChildElement("gid").text().toInt();
        QString name = node.firstChildElement("name").text();
        result.insert(-id, name);
        node = node.nextSiblingElement("group");
    }
    return result;
}

QList<QString> Vk::parseAttachments(QDomElement &e)
{
    QList<QString> result;
    QDomElement node = e.firstChildElement("attachments").firstChildElement("attachment");
    QDomElement innerNode;
    QString type;
    while(!node.isNull())
    {
        type = node.firstChildElement("type").text().trimmed();
        innerNode = node.firstChildElement().nextSiblingElement();
        if(type == "photo" || type == "posted_photo")
        {
            result<<innerNode.firstChildElement("src_big").text();
        }
        else if(type =="video")
        {
            QString link = "http://vk.com/video";
            link.append(innerNode.firstChildElement("owner_id").text().trimmed());
            link.append("_");
            link.append(innerNode.firstChildElement("vid").text().trimmed());
            result<<link;
        }
        else if(type == "audio")
        {
            QString name = "Audio: " + innerNode.firstChildElement("performer").text().trimmed()
                    + "-" + innerNode.firstChildElement("title").text().trimmed();
            result<<name;
        }
        node = node.nextSiblingElement();
    }
    return result;
}

