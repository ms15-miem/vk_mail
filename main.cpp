#include "client.h"
//#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    /*QApplication*/ QCoreApplication a(argc, argv);
//    a.setQuitOnLastWindowClosed(false);
    a.setApplicationName("gmail2vk");

    Client client;
    
    return a.exec();
}
