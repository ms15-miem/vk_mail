#ifndef GMAIL_H
#define GMAIL_H

#include <QObject>
#include <QDebug>

#include <QtGlobal>
#include <iostream>

#include <vmime/vmime.hpp>


#ifdef Q_OS_WIN32
#include <vmime/platforms/windows/windowsHandler.hpp>
#endif

#ifdef Q_OS_UNIX
#include <vmime/platforms/posix/posixHandler.hpp>
#endif

using namespace vmime::net;
using namespace std;


class myCertVerifier : public vmime::security::cert::certificateVerifier
{
public:

    void verify(vmime::ref<vmime::security::cert::certificateChain> chain)
    {
        // Obtain the subject's certificate
        vmime::ref <vmime::security::cert::certificate> cert = chain->getAt(0);

        std::cout << std::endl;
        std::cout << "Server sent a '" << cert->getType() << "'"
                  << " certificate." << std::endl;
        std::cout << "Do you want to accept this certificate? (Y/n) ";
        std::cout.flush();

        std::string answer;
        std::getline(std::cin, answer);

        if (answer.length() != 0 && (answer[0] == 'Y' || answer[0] == 'y'))
            return; // OK, we trush the certificate

        // Don't trust this certificate
        vmime::exceptions::certificate_verification_exception e
                ("Don't trust this certificate");
        throw e;
    }
};

class GMail : public QObject
{
    Q_OBJECT
public:
    explicit GMail(QString login, QString password, QObject *parent = 0);
    void connect();
private:
    void readEmails(vmime::ref<store> store);
    QString login, password;

signals:
    
public slots:
    
};

#endif // GMAIL_H
