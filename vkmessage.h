#ifndef VKMESSAGE_H
#define VKMESSAGE_H

#include <QObject>
#include "message.h"

class VkMessage : public Message
{
    Q_OBJECT
private:
    qint32 authorId;
    qint32 dateNumber;
    QList<QString> attachments;
    friend bool lessThanById(VkMessage* m1, VkMessage* m2);
    friend bool lessThanByDate(VkMessage* m1, VkMessage* m2);
public:
    qint32 getId();
    VkMessage(QString authorId, QString text, QDateTime dateTime);
    void setAuthor(QString name);
    void addAttachments(const QList<QString> &list);
    QString getText() const;
};

bool lessThanById(VkMessage* m1, VkMessage* m2);
bool lessThanByDate(VkMessage* m1, VkMessage* m2);



#endif // VKMESSAGE_H
