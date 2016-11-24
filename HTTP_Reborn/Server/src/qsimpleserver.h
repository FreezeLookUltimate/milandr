#ifndef QSIMPLESERVER_H
#define QSIMPLESERVER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDateTime>

class QSimpleServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit QSimpleServer();

    void takeRequest();
    void incomingConnection( qintptr handle);
signals:

public slots:
    void onReadyRead();
    void onDisconnected();

};

#endif // QSIMPLESERVER_H
