#include "client.h"

#include "vk.h"
#include "gmail.h"
#include "message.h"

#include <QDebug>

Client::Client(QObject *parent) :
    QObject(parent)
{
}

Client::~Client()
{
}

void Client::start()
{
    vk = new Vk(1, "3353341", true, this);
    gmail = new GMail(20, this);

    connect(vk, SIGNAL(connected()), SLOT(slotWork()));

//#ifdef QT_DEBUG
//    QObject::connect(gmail, SIGNAL(unreadedMessage(Message*)), SLOT(testSlot(Message*)));
//#else
    QObject::connect(gmail, SIGNAL(unreadedMessage(Message*)), vk, SLOT(slotPost(Message*)));
//#endif
    vk->connect();
}

void Client::slotWork()
{
    gmail->connect();
    gmail->startCheckCycle();
}

void Client::testSlot(Message *msg)
{
    qDebug() << msg->getText();
    msg->deleteLater();
}
