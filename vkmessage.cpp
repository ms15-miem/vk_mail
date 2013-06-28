#include <QNetworkReply>
#include <QDebug>
#include <QDateTime>
#include "vkmessage.h"


void VkMessage::setAuthor(QString name)
{
    this->from = name;
}

void VkMessage::addAttachments(const QList<QString> &list)
{
    attachments.append(list);
}

QString VkMessage::getText() const
{
    QString result =QString("["+from+"/vk]:"+body);
    QTextStream st(&result);
    for(int i =0; i<attachments.count();i++)
        st<<endl<<attachments.at(i);

    result.replace("<br>", "\n");

    return result;
}



bool lessThanById(VkMessage* m1, VkMessage* m2)
{
    return m1->authorId < m2->authorId;
}


bool lessThanByDate(VkMessage* m1, VkMessage* m2)
{
    return m1->dateTime < m2->dateTime;
}


qint32 VkMessage::getId()
{
    return authorId;
}

VkMessage::VkMessage(QString authorId, QString text, QDateTime dateTime)
    :Message(dateTime, authorId, "", text)
{
    this->authorId = authorId.toInt();
}
