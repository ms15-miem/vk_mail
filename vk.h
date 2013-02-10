#ifndef VK_H
#define VK_H

#include "oauth.h"

class Vk : public OAuth
{
    Q_OBJECT

    int clientId;

public:
    explicit Vk(int _clientId, QString _settingsGroup, QObject *parent = 0);
    ~Vk();

protected:
    void requestToken();

signals:

private slots:
    void slotUrlChanged(const QUrl &_url);

public slots:
    void slotPost(const Message &msg);
};

#endif // VK_H
