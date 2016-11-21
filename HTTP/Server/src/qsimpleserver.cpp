#include "qsimpleserver.h"
#include "requests.h"
QSimpleServer::QSimpleServer()
{
    if(listen(QHostAddress::Any, 8095)){
        qDebug() <<"Listening on " << 8095 << "...";
    } else{
        qDebug() <<"Error while starting: " << errorString();
    }
}

void QSimpleServer::incomingConnection(qintptr handle)
{
    QTcpSocket* socket = new QTcpSocket();
    socket->setSocketDescriptor(handle);
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

void QSimpleServer::onReadyRead()
{
    qDebug() << "NEW CONNECTION";
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray readed = socket->readAll();
    QString  request(readed);
    while(request.right(4)!="\r\n\r\n" && socket->state() != QTcpSocket::UnconnectedState){
        if(socket->bytesAvailable()>0) request.append(socket->read(1));
        else socket->waitForReadyRead(1);
    }
    QStringList header = request.split('\n');

    for(int i = 0; i <(header.length());i++){
    header[i].chop(1);
        if (header[i].isEmpty())
            header.removeAt(i);
    }
    header.removeLast();

    QStringList tokens=header[0].split(" ");
    QString POSTcontent;
    if(tokens[0] == "POST"){
        quint64 content_size=0,readed=0;
        QString content_desc;
        for(int i=0;i<header.size();i++){
            if(header[i].left(14).toUtf8()==QString("Content-Type: ").toUtf8())
                content_desc.append(header[i].mid(14).toUtf8());
            if(header[i].left(16).toUtf8()==QString("Content-Length: ").toUtf8())
                content_size=header[i].mid(16).toUtf8().toInt();
        }
        qDebug()<<"INPUT POST DATA LENGTH: "<<content_size;
        while(readed!=content_size && socket->state() != QTcpSocket::UnconnectedState){
            quint64 bytes_av=socket->bytesAvailable();
            if(bytes_av>0){
                quint64 bytes_left=content_size-readed;
                quint64 bytes_for_read=qMin(bytes_av,bytes_left);
                POSTcontent.append(socket->read(bytes_for_read));
                readed+=bytes_for_read;
            }
            if(readed<content_size){
                socket->waitForReadyRead(10);
            }
        }
        if(socket->state()==QTcpSocket::UnconnectedState){return;}
    }

    Requests* req = new Requests(header, POSTcontent);


    QString response = "HTTP/1.1 200 OK \r\n\r\n%1";
    QString argument = "Hello, Bro!";

    socket->write(response.arg(argument).toUtf8());
    socket->disconnectFromHost();
}

void QSimpleServer::onDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    qDebug() << "Socket close";
    socket->close();
    socket->deleteLater();
}
