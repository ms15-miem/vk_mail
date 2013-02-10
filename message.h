#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

class Message
{
    QString text;

public:
    Message(QString _text);
    QString getText() const;
};

#endif // MESSAGE_H
