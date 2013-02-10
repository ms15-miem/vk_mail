#include "client.h"

Client::Client(QObject *parent) :
    QObject(parent)
{
    vk = new Vk(3353341, this);
    vk->setKeepAuth(true);

    gmail = new GMail(this);

    connect(vk, SIGNAL(setReady(bool)), SLOT(slotWork(bool)));
    connect(gmail, SIGNAL(setReady(bool)), SLOT(slotWork(bool)));
    connect(gmail, SIGNAL(unreadedMessage(Message)), vk, SLOT(slotPost(Message)));
    vk->connect();
    gmail->connect();

    gmail->test();
}

Client::~Client()
{
}

void Client::slotWork(bool ready)
{
    if (qobject_cast<Vk*>(sender())) {
        vkReady = ready;
    }
    else if (qobject_cast<GMail*>(sender())) {
        gmailReady = ready;
    }

    if (vkReady && gmailReady) {
        gmail->startCheckCycle();
    }
}
