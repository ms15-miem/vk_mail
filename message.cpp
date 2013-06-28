#include "message.h"

Message::Message(QDateTime _date, QString _from, QString _subj, QString _body, QObject *pobj)
    :QObject(pobj)
{
    dateTime = _date;
    from = _from;
    subj = _subj;
    body = _body;
}

QString Message::getText() const
{
    QString ENDL("\n");
    return  dateTime.toString()+ENDL+
            QString("New message from ")+from+ENDL+
            QString("Subject:")+subj+ENDL+
            body;
}

QString Message::getAttachments() const
{
//TODO
    return QString("\nAttachments:\n");
}


Message::Message()
{
}
