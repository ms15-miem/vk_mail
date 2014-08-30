#include "client.h"

#include <QCoreApplication>
#include <QTextCodec>

#include <exception>

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);

    QCoreApplication a(argc, argv);
    a.setApplicationName("gmail2vk");

    try
    {
        Client client;
        client.start();
        a.exec();
    }
    catch(std::exception &e)
    {
        qFatal(e.what());
    }
    catch(...)
    {
        qFatal("Exception has been caught");
    }
    return 0;
}
