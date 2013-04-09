#include <QCoreApplication>

#include "gmail.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        qDebug() << "program needs arguments for gmail: login password";
        exit(1);
    }
    GMail gmail(argv[1], argv[2]);
    gmail.connect();
}
