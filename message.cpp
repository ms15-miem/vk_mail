#include "message.h"

Message::Message(QString text):date(),from(),subj(),body(text)
{

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
