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

    result.replace("<br><br>", "\n");

    return result;
}



bool lessThanById(VkMessage* m1, VkMessage* m2)
{
    return m1->authorId < m2->authorId;
}


bool lessThanByDate(VkMessage* m1, VkMessage* m2)
{
    return m1->date < m2->date;
}


qint32 VkMessage::getId()
{
    return authorId;
}

VkMessage::VkMessage(QString authorId, QString text, QString date):Message(QDateTime::fromTime_t(date.toInt()).toString(), "", text)
{
    this->authorId = authorId.toInt();
}
