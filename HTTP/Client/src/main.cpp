#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTextStream>

void sendRequestGET();
void sendRequestPOST();

int main(int argc, char *argv[])
{
    QString command;
    QCoreApplication a(argc, argv);
    QTextStream cin(stdin);
    qDebug() << "Данная программа - тестовый клиент" <<endl
         << "Доступные команды:" <<endl
         << "post - отправка POST запроса на сервер localhost:8095" << endl
         << "get - отправка GET запроса на сервер localhost:8095" << endl
         << "exit - выход из программы"<<endl
         << "Пока что так. Ввод:";
    while(true){
    cin >> command;
    command = command.toLower();


        if (command.contains("post"))
        {
            qDebug() << "Идет отправка POST запроса";
            sendRequestPOST();
            return a.exec();
        }
        else if (command.contains("get"))
        {
            qDebug() << "Идет отправка GET запроса";
            sendRequestGET();
            return a.exec();
        }
        else if (command.contains("exit"))
        {
            qDebug() << "Выход.";
            return 0;
        } else {
             qDebug() << "Невалидная команда, повторите ввод";
        }
    }
}

void sendRequestGET(){

    QEventLoop eventLoop;

    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req( QUrl( QString("http://localhost:8095") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // Пока Finished() не выполнился

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Success!" <<reply->readAll();
        delete reply;
    }
    else {
        //failure
        qDebug() << "Fail" <<reply->errorString();
        delete reply;
    }
}

void sendRequestPOST(){

    QEventLoop eventLoop;

    QUrl serviceUrl = QUrl( QString("http://localhost:8095") );
    QNetworkRequest req(serviceUrl);
    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    QObject::connect(mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QString data;
    for(int i = 0; i < 10; i++){
        data.append("Argument=1\n");

    }
    QNetworkReply *reply = mgr->post(req, data.toLatin1().data());
     eventLoop.exec();
     if (reply->error() == QNetworkReply::NoError) {
         qDebug() << "Success!" <<reply->readAll();
         delete reply;
     }
     else {
         //failure
         qDebug() << "Fail" <<reply->errorString();
         delete reply;
     }
}

