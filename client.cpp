#include "client.h"

Client::Client(QObject *parent) :
    QObject(parent), vkReady(false), gmailReady(false)
{
    vk = new Vk("3353341", "vk", this);
    vk->setKeepAuth(true);

//    gmail = new GMail("nJI6O41-oDgtcMA9Q_MmiyVt", "urn:ietf:wg:oauth:2.0:oob", "479175816888.apps.googleusercontent.com",
//                      "gmail", this);

//    gmail->setKeepAuth(true);

    connect(vk, SIGNAL(setReady(bool)), SLOT(slotWork(bool)));
//    connect(gmail, SIGNAL(setReady(bool)), SLOT(slotWork(bool)));
//    connect(gmail, SIGNAL(unreadedMessage(Message*)), vk, SLOT(slotPost(Message*)));
    vk->connect();
//    gmail->connect();

}

Client::~Client()
{
}

void Client::slotWork(bool ready)
{
    if (qobject_cast<Vk*>(sender())) {
        vkReady = ready;
        vk->slotStartCheckCycle();
        QObject::connect(vk, SIGNAL(unreadedMessage(Message*)), this, SLOT(testSlot(Message*)));
    }
//    else if (qobject_cast<GMail*>(sender())) {
//        gmailReady = ready;
//    }

    if (vkReady && gmailReady) {
        gmail->slotStartCheckCycle();
    }
}

void Client::testSlot(Message* msg)
{
    qDebug()<<"text"<<msg->getText();
}
