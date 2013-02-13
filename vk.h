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
    void getAcceptToken();
    void saveAuthData() const;
    void loadAuthData();
    bool isAuthDataReady() const;

signals:

private slots:
    void slotUrlChanged(const QUrl &_url);

public slots:
    void slotPost(const Message &msg);
};

#endif // VK_H
