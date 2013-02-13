#ifndef VK_H
#define VK_H

#include "oauth.h"

class Vk : public OAuth
{
    Q_OBJECT

public:
    explicit Vk(QString _clientId, QString _settingsGroup, QObject *parent = 0);
    ~Vk();
    void connect();

protected:
    void saveAuthData() const;
    void loadAuthData();

signals:

private slots:
    void slotUrlChanged(const QUrl &_url);

protected slots:
    void slotGetAccessToken();

public slots:
    void slotPost(const Message &msg);
};

#endif // VK_H
