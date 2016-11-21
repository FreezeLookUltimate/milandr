#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

void sendRequestGET();
void sendRequestPOST();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    sendRequestPOST();//Не работает потому что клиент не формирует запрос
    return a.exec();
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
        data.append("Qt 4 is several years old and contains a version of WebKit with many serious defects. Support for Qt 4 will be dropped in any future feature releases and is not supported by the capybara-webkit team. If you add instructions to this page, please make sure your instructions build using Qt 5.");
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

