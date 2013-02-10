#include "client.h"

Client::Client(QObject *parent) :
    QObject(parent)
{
    vk = new Vk(3353341, this);
    connect(vk, SIGNAL(receivedAccessToken()), this, SLOT(slotPost()));
    vk->connect();
}

void Client::slotPost()
{
    vk->slotPost(Message());
}
