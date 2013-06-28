#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "vk.h"
#include "gmail.h"

class Client : public QObject
{
    Q_OBJECT

    Vk *vk;
    GMail *gmail;
    bool vkReady;

public:
    explicit Client(QObject *parent = 0);
    ~Client();
    
signals:

private slots:
    void slotWork(bool ready);
    void testSlot(Message* msg);

public slots:
    
};

#endif // CLIENT_H
