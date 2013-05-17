#include "client.h"

Client::Client(QObject *parent) :
    QObject(parent)/*, vkReady(false), gmailReady(false)*/
{
    vk = new Vk(1, "3353341", "vk", this);
    vk->setKeepAuth(true);

    connect(vk, SIGNAL(setReady(bool)), SLOT(slotWork(bool)));

    gmail = new GMail(1, this);

    QObject::connect(gmail, SIGNAL(unreadedMessage(Message*)), vk, SLOT(slotPost(Message*)));


    vk->connect();
    gmail->connect();
}

Client::~Client()
{
}

void Client::slotWork(bool ready)
{
    vkReady = ready;
    //    vk->slotStartCheckCycle();
    //    QObject::connect(vk, SIGNAL(unreadedMessage(Message*)), this, SLOT(testSlot(Message*)));
    gmail->startCheckCycle();
}

void Client::testSlot(Message *msg)
{
    qDebug()<<"text"<<msg->getText();
    msg->deleteLater();
}
