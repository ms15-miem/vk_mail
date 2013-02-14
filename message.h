#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

class Message
{
    QString date;   // Mon, 11 Feb 2013 21:22:08 +0400
    QString from; // =?UTF-8?B?0J/RgNC+0LXQutGCIFFJUA==?= <noreply@copiny.com>
    //QString To; // user@domain.ru
    QString subj; //UTF-8
    QString body;  //UTF-8

public:
    Message(QString _text);
    QString getText() const;
    QString getAttachments() const;
};

#endif // MESSAGE_H
