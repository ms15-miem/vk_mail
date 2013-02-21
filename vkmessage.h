#ifndef VKMESSAGE_H
#define VKMESSAGE_H

#include <QObject>
#include "message.h"

class VkMessage : public QObject
{
    Q_OBJECT
private:
    QString authorId;
    QString text;
    QString date;
public:
    VkMessage(QString authorId, QString text, QString date, QObject *parent = 0);
    
signals:
    void messageReady(Message msg);
    
public slots:
    void slotUserRequestFinished();
    void slotGroupRequestFinished();
};

#endif // VKMESSAGE_H
