#ifndef GMAILMESSAGE_H
#define GMAILMESSAGE_H

#include "message.h"

class GMailMessage : public Message
{
    Q_OBJECT
public:
    explicit GMailMessage(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // GMAILMESSAGE_H
