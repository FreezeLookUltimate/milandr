#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTextStream>
#include <QStringList>
#include <QFile>

void sendRequestGET();
void sendRequestPOST(QString name, QString data);
int parsingCommand(QString command);


QStringList instruction;
QTextStream cin(stdin);
QTextStream cout(stdout);
QString address = "http://localhost:8095";
QString attribute = "";
bool onLine = false;

int parsingCommand(QString command){
    attribute.clear();


    if (command[0] != '-'){
        cout << "Incorrect command. Example:" <<endl
            << "-[Command] [parameter1] [parameter2] .." << endl
            << "Try again." << endl;
        return 0;
    }


    instruction = command.split(" ");
    if (instruction[0] == "-connect" && instruction.length()==3)
    {
        address = "http://";
        address.append(instruction[1]);
        address.append(':');
        address.append(instruction[2]);
        cout << "Address now " << address << endl;
    }


    if (instruction[0] == "-connect"){
        cout << "Go connect" << endl;
        sendRequestGET();
        return 0;
    }

    if(instruction[0]=="-exit")
        return 1;

    if (onLine == true && instruction[0]!="-pushfile"){
        attribute.append('/');
        attribute.append(instruction[0].mid(1));
        attribute.append('&');
        if(instruction.length()>1){
            for(int i = 1; i < instruction.length(); i++){
                attribute.append(instruction[i]);
                attribute.append('&');
            }
            attribute.chop(1);
        }
        sendRequestGET();
        return 0;
    }

    if (onLine == true && instruction[0]=="-pushfile"){

        if (instruction[1].isEmpty()){
            cout << "Incorrect command. Example:"
                 << "-pushfile [name](with file format)" << endl
                 << "Try again." << endl;
            return 0;
        }


        if(QFile::exists("./" + instruction[1])){
            QFile file("./" + instruction[1]);
            file.open(QIODevice::ReadOnly);
            QString output(file.readAll());
            file.close();
            sendRequestPOST(instruction[1], output);

        } else {
            cout << "File not found, check command";
            return 0;
        }


    }

    return 0;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString command;


    cout << "This programm - test client" <<endl
         << "Commands:" <<endl
         << "-connect             ::Connect to 127.0.0.1 on port 8095" <<endl
         << "-connect [ip] [port] ::Connect to [ip] on port [port]" <<endl
         << "-exit                ::Quit the program" <<endl;



    int i = 0;
    while(i!=1){
        cout << "Push: " << endl;
        command = cin.readLine();
        command.toLower();
        i = parsingCommand(command);
    }


    a.exit(0);
    return 0;
}

void sendRequestGET(){

    QEventLoop eventLoop;

    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req( QUrl( QString(address + attribute) ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // Пока Finished() не выполнился

    if (reply->error() == QNetworkReply::NoError) {
        if (instruction[0] != "-getfile"){
            cout <<reply->readAll()<< endl;
        }
        else{
            QFile getfile(instruction[1]);
            getfile.open(QIODevice::WriteOnly);
            getfile.write(reply->readAll());
            getfile.close();
            cout << "File " << instruction[1] << " downloaded." << endl;
        }

        onLine = true;
        delete reply;
    }
    else {
        //failure
        cout<< "Critical error: " <<reply->errorString()<<endl;
        onLine = false;
        delete reply;
    }
}

void sendRequestPOST(QString name, QString data){

    QEventLoop eventLoop;
    QUrl serviceUrl = QUrl( QString(address + "/file?" + name) );
    QNetworkRequest req(serviceUrl);
    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    QObject::connect(mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkReply *reply = mgr->post(req, data.toStdString().c_str());
     eventLoop.exec();
     if (reply->error() == QNetworkReply::NoError) {
         cout <<reply->readAll();
         delete reply;
     }
     else {
         //failure
         cout << "Critical error: " <<reply->errorString();
         delete reply;
     }
}

