#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

class Vk;
class GMail;
class Message;

class Client : public QObject
{
    Q_OBJECT

    Vk *vk;
    GMail *gmail;

public:
    explicit Client(QObject *parent = 0);
    ~Client();
    void start();
    
signals:

private slots:
    void slotWork();
    void testSlot(Message* msg);

public slots:
    
};

#endif // CLIENT_H
