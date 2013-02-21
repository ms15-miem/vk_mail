#include "message.h"

Message::Message(QString text):date(),from(),subj(),body(text)
{

}

Message::Message(QString _date, QString _from, QString _text):subj()
{
    date = _date;
    from = _from;
    body = _text;
}

QString Message::getText() const
{
    QString ENDL("\n");
    return  date+ENDL+
            QString("New message from ")+from+ENDL+
            QString("Subject:")+subj+ENDL+
            body;
}

QString Message::getAttachments() const
{
//TODO
    return QString("\nAttachments:\n");
}
