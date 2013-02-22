#ifndef VK_H
#define VK_H

#include <QDomDocument>
#include "oauth.h"
#include "vkmessage.h"
#include "message.h"

class Vk : public OAuth
{
    Q_OBJECT

private:
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
    void getMessages();
    void returnMessages();
public:
    explicit Vk(QString _clientId, QString _settingsGroup, QObject *parent = 0);
    ~Vk();
    void connect();
    void setCheckInterval(int minutes);
    int getCheckInterval();


protected:
    void saveAuthData() const;
    void loadAuthData();

signals:
    void unreadedMessage(Message* msg);

private slots:
    void slotUrlChanged(const QUrl &_url);
    void slotCheckMessages();
    void slotCheckRequestFinished();

protected slots:
    void slotGetAccessToken();
    void slotMessagesRequestFinished();

public slots:
    void slotPost(const Message &msg);
    void slotStartCheckCycle();
};

#endif // VK_H
