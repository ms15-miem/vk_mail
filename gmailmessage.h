#ifndef GMAILMESSAGE_H
#define GMAILMESSAGE_H

#include "message.h"
#include <QTextStream>

class GMailMessage : public Message
{
    Q_OBJECT
public:
    explicit GMailMessage();
    GMailMessage(QDateTime _date, QString _from, QString _subj, QString _body, QObject *pobj = 0);
    QString getText() const;

private:
    
signals:
    
public slots:
    
};

#endif // GMAILMESSAGE_H
