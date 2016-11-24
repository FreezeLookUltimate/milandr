#include "qsimpleserver.h"
#include "requests.h"
#include <QJsonDocument>
#include <QDir>
QSimpleServer::QSimpleServer()
{
    if(listen(QHostAddress::Any, 8095)){
        qDebug() <<"Listening on " << 8095 << "...";
    } else{
        qDebug() <<"Error while starting: " << errorString();
    }
    bool check = Requests :: CheckFiles();
    if (!check)
        Requests::FirstServerRun();
}

void QSimpleServer::takeRequest()
{

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
    //Новое соедининие
    //
    //Новое соединение
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray readed = socket->readAll();
    QString  request(readed);

    qDebug() << "СОКЕТ ЕСТЬ";
    //Читаем полностью заголовок
    //
    //Читаем полностью заголовок
    while(request.right(4)!="\r\n\r\n" && socket->state() != QTcpSocket::UnconnectedState){
        if(socket->bytesAvailable()>0) request.append(socket->read(1));
        else socket->waitForReadyRead(1);
    }
    qDebug() << "Заголовки прочитаны!";

    //Парсим хедеры
    //
    //Парсим хедеры
    QStringList header = request.split('\n');
    for(int i = 0; i <(header.length());i++){
    header[i].chop(1);
        if (header[i].isEmpty())
            header.removeAt(i);
    }
    header.removeLast();

    qDebug() << "Хедеры распарсены";

    QStringList tokens=header[0].split(" ");


    //Если все же POST - забираем полностью
    //
    //Если все же POST - забираем полностью
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
        Requests ::CreateFile(tokens[1].mid(1), POSTcontent);
    }

    Requests::writeJSON(socket->peerAddress().toString(),socket->peerPort(), header, POSTcontent);

    //Парсим параметры
    //
    //Парсим параметры
    QStringList parametrs;
    if (tokens[1] == "/")
        qDebug() << "Запрос на подключение";
    qDebug() << "Параметры распарсены";

    QStringList response;
    response = Requests :: readJSON("error");


    //Разбираемся с параматрами запроса для GET
    //
    //Разбираемся с параметрами запроса для GET

    qDebug() << tokens[0];
    if(tokens[0] == "GET"){
        qDebug() << "TEST GET";
        if (tokens[1]=="/")
        {
            response = Requests :: readJSON("mainpage");
        } else {
            parametrs = tokens[1].split("&");
            qDebug() << parametrs << "Параметры";
            if (parametrs[0] == "/info")
            {
                qDebug() << "Кидаем info";
                response = Requests :: readJSON("info");
            }

            if (parametrs[0] == "/register"){
                response[0] = "HTTP/1.1 200 OK \r\n\r\n%1";
                response[1] = Requests::Register(parametrs[1], parametrs[2], socket->peerAddress().toString(), socket->peerPort());
            }
            if (parametrs[0] == "/signin")
                if (Requests :: SingIn(parametrs[1], parametrs[2])){
                    response = Requests :: readJSON("userpage");
                } else {
                    response[0] = "HTTP/1.1 200 OK \r\n\r\n%1";
                    response[1] = "Login or password incorrect. Try again";
                }
            if (parametrs[0] == "/filelist")
            {
                response[0] = "HTTP/1.1 200 OK \r\n\r\n%1";
                response[1] = Requests::FileList();
            }
            if (parametrs[0] == "/getfile"){
                response[0] = "HTTP/1.1 200 OK \r\n\r\n%1";
                response[1] = Requests ::TakeFile(parametrs[1]);
            }
        }




    }

    if(tokens[0] == "POST")
    {
        qDebug() << "TEST POST!";
        if (tokens[1]=="/")
        {
            response = Requests :: readJSON("error");
        }
        else
        {
            parametrs = tokens[1].split("?");
            qDebug() << parametrs;
            Requests::CreateFile(parametrs[1], POSTcontent);
            response[0] = "HTTP/1.1 200 OK \r\n\r\n%1";
            response[1] = "File added";
        }
    }



    socket->write(response[0].arg(response[1]).toUtf8());
    qDebug() << "Пишем ответ";
    socket->disconnectFromHost();
}

void QSimpleServer::onDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    qDebug() << "Socket close";
    socket->close();
    socket->deleteLater();
}
