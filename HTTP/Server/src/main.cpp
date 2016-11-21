#include <QCoreApplication>
#include "qsimpleserver.h"
#include "requests.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSimpleServer server;
    return a.exec();
}
