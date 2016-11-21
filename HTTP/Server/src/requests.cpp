#include "requests.h"


void Requests::newClient()
{
    QJsonObject jsonGetObj;
    jsonGetObj["Data"] = "THIS IS.....GET!";
    jsonGetObj["Header"] = "HTTP/1.1 200 OK \r\n\r\n%1";

    QJsonDocument jsonGetDoc(jsonGetObj);
    QString str = "GET";
    QFile saveGet("./Requests/Output/" + str + ".json");

    saveGet.open(QIODevice::WriteOnly);
    saveGet.write(jsonGetDoc.toJson());
    saveGet.close();

    QJsonObject jsonPostObj;
    jsonPostObj["Data"] = "THIS IS.....POST!";
    jsonPostObj["Header"] = "HTTP/1.1 200 OK \r\n\r\n%1";
    QJsonDocument jsonPostDoc(jsonPostObj);

    str = "POST";
    QFile savePost("./Requests/Output/" + str + ".json");
    savePost.open(QIODevice::WriteOnly);
    savePost.write(jsonPostDoc.toJson());
    savePost.close();

}

void Requests::writeJSON()
{
    QJsonObject jsonObj;

    QString name;
    QString arg;
    jsonObj["Type"] = type;
    for(int i = 1; i < headerslist.length(); i++)
    {
        int k = headerslist[i].indexOf(':');
        name = headerslist[i].left(k);
        arg = headerslist[i].right(headerslist[i].length() - k - 2);
        jsonObj[name] = arg;
    }
    if (type == "POST")
        jsonObj["Data"] = content;
    QJsonDocument jsonDoc(jsonObj);
    QFile saveJson("./Requests/Input/" + requestHost + ":" + type +":" +  QDateTime::currentDateTime().toString("dd.MM.yyyy").toUtf8().data() + ":" +  QDateTime::currentDateTime().toString("hh.mm.ss").toUtf8().data() + ".json");
    saveJson.open(QIODevice::WriteOnly);
    saveJson.write(jsonDoc.toJson());
    saveJson.close();
}

Requests::Requests(QStringList headers, QString str)
{
    newClient();

    headerslist = headers;
    content = str;
    for(int i = 0; i < headerslist.length(); i++){
        qDebug() <<headerslist[i];
    }
    for(int i = 0; i < headerslist.length(); i++){
        if(headerslist[i].contains("GET"))
            type = "GET";
        else if(headerslist[i].contains("POST"))
            type = "POST";
        else if(type.isEmpty())
            type = "UNDIFINED";

        if(headerslist[i].contains("Host"))
        {
            int k = headerslist[i].indexOf(':');
            requestHost = headerslist[i].right(headerslist[i].length()-k-2);
        }
    }
    writeJSON();


    qDebug() << "POST DATA: " <<str;
    qDebug() << "POST DATA LENGTH: "<< content.length();

}

QStringList Requests::readJSON()
{
    QStringList output;
    QFile loadJson("./Requests/Output/" + type + ".json");
    loadJson.open(QIODevice::ReadOnly);
    QByteArray loadData = loadJson.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(loadData));
    QJsonObject jsonObj = jsonDoc.object();
    output << jsonObj["Header"].toString() << jsonObj["Data"].toString();
    loadJson.close();
    return output;
}
