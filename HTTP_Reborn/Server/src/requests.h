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

public:
    static bool CheckFiles();
    static void FirstServerRun();
    static void writeJSON(QString ip, quint16 port, QStringList headerslist, QString content);
    static QString TakeFile(QString name);
    void CreateUser(QString name, QString pass, QString ip, QString port);
    static void CreateFile(QString name, QString content);
    static QString FileList();
    static QString Register(QString username, QString password, QString ip, quint16 port);
    static bool SingIn(QString username, QString password);


    Requests();
    static QStringList readJSON(QString name);
};

#endif // REQUESTS_H
