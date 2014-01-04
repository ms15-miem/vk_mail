#include "gmailmessage.h"

#include <QTextStream>

GMailMessage::GMailMessage()
{
}

GMailMessage::GMailMessage(QDateTime _date, QString _from, QString _subj, QString _body, QObject *pobj)
    :Message(_date, _from, _subj, _body, pobj)
{
}

QString GMailMessage::getText() const
{
    QString ans;
    QTextStream out(&ans, QIODevice::WriteOnly);
    out << "From: " << from << "\n"
        << "Date: " << dateTime.toString() << "\n"
        << "Subject: " << subj << "\n"
        << "\n"
           << body;
    return ans;
}
