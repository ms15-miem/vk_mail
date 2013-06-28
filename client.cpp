#include "client.h"

Client::Client(QObject *parent) :
    QObject(parent), vkReady(false)
{
    vk = new Vk(1, "3353341", this);
    vk->setKeepAuth(true);

    connect(vk, SIGNAL(setReady(bool)), SLOT(slotWork(bool)));

    gmail = new GMail(1, this);

#ifdef QT_DEBUG
    QObject::connect(gmail, SIGNAL(unreadedMessage(Message*)), SLOT(testSlot(Message*)));
//            QObject::connect(gmail, SIGNAL(unreadedMessage(Message*)), vk, SLOT(slotPost(Message*)));
#else
        QObject::connect(gmail, SIGNAL(unreadedMessage(Message*)), vk, SLOT(slotPost(Message*)));
#endif


    vk->connect();
    gmail->connect();
}

Client::~Client()
{
}

void Client::slotWork(bool ready)
{
    vkReady = ready;
    gmail->startCheckCycle();
}

void Client::testSlot(Message *msg)
{
    qDebug()<<"text"<<msg->getText();
    msg->deleteLater();
}
