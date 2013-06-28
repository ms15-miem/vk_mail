#include "client.h"
//#include <QApplication>
#include <QCoreApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);

    /*QApplication*/ QCoreApplication a(argc, argv);
//    a.setQuitOnLastWindowClosed(false);
    a.setApplicationName("gmail2vk");

    Client client;
    
    return a.exec();
}
