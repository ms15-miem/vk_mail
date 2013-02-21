#include <QNetworkReply>
#include <QDebug>
#include "vkmessage.h"


void VkMessage::slotUserRequestFinished()
{
    QNetworkReply *rep = qobject_cast<QNetworkReply*>(sender());
    qDebug()<<rep->readAll();
}

void VkMessage::slotGroupRequestFinished()
{
    QNetworkReply *rep = qobject_cast<QNetworkReply*>(sender());
    qDebug()<<rep->readAll();
}


VkMessage::VkMessage(QString authorId, QString text, QString date, QObject *parent):QObject(parent)
{
    this->authorId = authorId;
    this->text = text;
    this->date = date;
}
