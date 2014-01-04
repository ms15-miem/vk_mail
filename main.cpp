#include "client.h"
//#include <QApplication>
#include <QCoreApplication>
#include <QTextCodec>

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
    catch(...)
    {
        qFatal("Exception has been caught");
    }
    return 0;
}
