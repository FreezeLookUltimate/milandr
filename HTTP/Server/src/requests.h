#ifndef REQUESTS_H
#define REQUESTS_H

#include <QByteArray>
#include <QStringList>
#include <QDebug>
class Requests
{
private:
    QStringList headerslist;
    QString content;
public:
    Requests(QStringList headers, QString str);
};

#endif // REQUESTS_H
