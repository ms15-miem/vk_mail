#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "vk.h"
#include "gmail.h"

class Client : public QObject
{
    Q_OBJECT

    Vk *vk;
public:
    explicit Client(QObject *parent = 0);
    
signals:

private slots:
    void slotPost();

public slots:
    
};

#endif // CLIENT_H
