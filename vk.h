#ifndef VK_H
#define VK_H

#include "oauth.h"

class QTimer;
class QDomDocument;
class VkMessage;
class QDomElement;
class Message;

class Vk : public OAuth
{
    Q_OBJECT

private:
    static const QString groupId;
    QList<VkMessage*> pendingMessages;
    qint32 lastId;
    qint32 nextLastId;
    qint32 nextLastDate;
    qint32 checkIntervalMinutes;
    qint32 lastDate;
    qint32 offset;
    QTimer* refreshTimer;
    QList<VkMessage*> parseMessages(QDomDocument& xml, QHash<qint32, QString>& names, bool* fin);
    QHash<qint32,QString> parseUsers(QDomDocument& xml);
    QHash<qint32,QString> parseGroups(QDomDocument& xml);
    QList<QString> parseAttachments(QDomElement &e);
    void getMessages();
    void returnMessages();
    void saveAuthData() const;

protected:
    void loadAuthData();
    void saveSettings();

public:
    explicit Vk(int checkIntervalMinutes, QString _clientId, bool keepAuth, QObject *parent = 0);
    ~Vk();
    void connect();
    void setCheckInterval(int minutes);
    int getCheckInterval();

signals:
    void unreadedMessage(Message* msg);

private slots:
    void slotCheckMessages();
    void slotCheckRequestFinished();
    void slotGetAccessToken();
    void slotMessagesRequestFinished();

public slots:
    void slotPost(Message *msg);
    void slotStartCheckCycle();
};

#endif // VK_H
