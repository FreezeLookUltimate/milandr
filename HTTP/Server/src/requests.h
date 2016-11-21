#ifndef REQUESTS_H
#define REQUESTS_H

 #include <QIODevice>
#include <QByteArray>
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDateTime>
#include <QDebug>
class Requests
{
private:
    QString type;
    QString requestHost;
    QStringList headerslist;
    QString content;

    void newClient();
    void writeJSON();
public:
    Requests(QStringList headers, QString str);
    QStringList readJSON();
};

#endif // REQUESTS_H
