#include "message.h"

Message::Message(QString _text):text(_text)
{
}

QString Message::getText() const
{
    return text;
}
