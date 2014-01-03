#include "client.h"

Client::Client(QObject *parent) :
    QObject(parent), vkReady(false)
{
    vk = new Vk(1, "3353341", true, this);
    gmail = new GMail(20000, this);
    vk->setKeepAuth(true);

    connect(vk, SIGNAL(connected()), SLOT(slotWork()));

#ifdef QT_DEBUG
    QObject::connect(gmail, SIGNAL(unreadedMessage(Message*)), SLOT(testSlot(Message*)));
#else
    QObject::connect(gmail, SIGNAL(unreadedMessage(Message*)), vk, SLOT(slotPost(Message*)));
#endif
    vk->connect();
}

Client::~Client()
{
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
