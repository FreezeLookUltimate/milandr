#include "requests.h"
#include <QDir>

bool Requests::CheckFiles(){
    qDebug() << "Проверка файлов...";
    if ( ! QFile::exists("./Requests/Output/loaded.json") )
        return false;
    if ( ! QFile::exists("./Requests/Output/info.json") )
        return false;
    if ( ! QFile::exists("./Requests/Output/mainpage.json") )
        return false;
    if ( ! QFile::exists("./Requests/Output/userpage.json") )
        return false;
    if ( ! QFile::exists("./Requests/Output/error.json") )
        return false;
    return true;
}

void Requests::FirstServerRun()
{
    qDebug() << "Сервер запущен впервые, создаем папки" <<  endl;
    QDir().mkdir("Requests");
    QDir().mkdir("./Requests/Input");
    QDir().mkdir("./Requests/Output");
    QDir().mkdir("Files");
    QDir().mkdir("Users");



    QString str;

    // Create "Loaded"
    //
    // Create "Loaded"
    QJsonObject jsonLoadedObj;
    jsonLoadedObj["Data"] = "File Added\n";

    jsonLoadedObj["Header"] = "HTTP/1.1 200 OK \r\n\r\n%1";

    QJsonDocument jsonLoadedDoc(jsonLoadedObj);
    str = "loaded";
    QFile saveLoaded("./Requests/Output/" + str + ".json");
    saveLoaded.open(QIODevice::WriteOnly);
    saveLoaded.write(jsonLoadedDoc.toJson());
    saveLoaded.close();
    // Create "Loaded" end
    //
    // Create "Loaded" end

    // Create "Error"
    //
    // Create "Error"
    QJsonObject jsonErrorObj;
    jsonErrorObj["Data"] = "Server error. Contact with servise server specialist.\n";

    jsonErrorObj["Header"] = "HTTP/1.1 200 OK \r\n\r\n%1";

    QJsonDocument jsonErrorDoc(jsonErrorObj);
    str = "error";
    QFile saveError("./Requests/Output/" + str + ".json");
    saveError.open(QIODevice::WriteOnly);
    saveError.write(jsonErrorDoc.toJson());
    saveError.close();
    // Create "Error" end
    //
    // Create "Error" end


    // Create "Info" end
    //
    // Create "Info" end
    QJsonObject jsonInfoObj;
    str = "";
    str.append("____________СССССС________\n");
    str.append("__________CCCCCCCCCC_____\n");
    str.append("________CCCCCCCCCCCCC____\n");
    str.append("_______CCCCCCCCCCC..CC____\n");
    str.append("______CCCCCCCCCCCC..CC____\n");
    str.append("______CCCCCCCCCCCC.CCC____\n");
    str.append("______CCCCCCCCCCC.CCC_____\n");
    str.append("______CCCCCCCCCCCCCC______\n");
    str.append("______CCCCCCCCCCCC________\n");
    str.append("_______CCCCCCCC___________\n");
    str.append("________CC_________________\n");
    str.append("_______CCC_________________\n");
    str.append("________*__________________\n");
    str.append("________*_________________\n");
    str.append("_________*________________\n");
    str.append("__________*_______________\n");
    str.append("__________*_______________\n");
    str.append("_________*________________\n");
    str.append("________*_________________\n");
    str.append("_______*__________________\n");
    str.append("_____WE TESTING THIS PAGE_\n");


    jsonInfoObj["Data"] = str;
    jsonInfoObj["Header"] = "HTTP/1.1 200 OK \r\n\r\n%1";

    QJsonDocument jsonInfoDoc(jsonInfoObj);
    str = "info";
    QFile saveInfo("./Requests/Output/" + str + ".json");
    saveInfo.open(QIODevice::WriteOnly);
    saveInfo.write(jsonInfoDoc.toJson());
    saveInfo.close();
    // Create "Info" end
    //
    // Create "Info" end


    // Create "MainPage" end
    //
    // Create "MainPage" end
    QJsonObject jsonMainObj;
    str = "";
    str.append("**********************\n");
    str.append("Welcome to HTTPServer.\n");
    str.append("**********************\n");
    str.append("Available commands:\n");
    str.append("-info                           :: Information about server\n");
    str.append("-register [login] [password]	:: Registration on a server\n");
    str.append("-signin [login] [password]      :: Sign In\n");
    str.append("-exit                           :: Quit the program\n");

    jsonMainObj["Data"] = str;
    jsonMainObj["Header"] = "HTTP/1.1 200 OK \r\n\r\n%1";

    QJsonDocument jsonMainDoc(jsonMainObj);
    str = "mainpage";
    QFile saveMain("./Requests/Output/" + str + ".json");
    saveMain.open(QIODevice::WriteOnly);
    saveMain.write(jsonMainDoc.toJson());
    saveMain.close();
    // Create "MainPage" end
    //
    // Create "MainPage" end





    // Create "UserPage" end
    //
    // Create "UserPage" end
    QJsonObject jsonUserObj;
    str = "";
    str.append("**********************\n");
    str.append("Hello, user!.\n");
    str.append("**********************\n");
    str.append("Available commands for users:\n");
    str.append("-info                   :: Information about server\n");
    str.append("-filelist              :: Take list of files\n");
    str.append("-getfile [name] 		:: Download file\n");
    str.append("-pushfile [name]		:: Add you file to the server\n");
    str.append("-exit                   :: Quit the program\n");

    jsonUserObj["Data"] = str;

    jsonUserObj["Header"] = "HTTP/1.1 200 OK \r\n\r\n%1";
    QJsonDocument jsonUserDoc(jsonUserObj);
    str = "userpage";
    QFile saveUser("./Requests/Output/" + str + ".json");
    saveUser.open(QIODevice::WriteOnly);
    saveUser.write(jsonUserDoc.toJson());
    saveUser.close();
    // Create "UserPage" end
    //
    // Create "UserPage" end


}

void Requests::writeJSON(QString ip, quint16 port, QStringList headerslist, QString content)
{

    QJsonObject jsonObj;
    QString name;
    QString arg;
    QString type;

    //Поиск типа запроса
    //
    //Поиск типа запроса
    if(headerslist[0].contains("GET"))
        type = "GET";
    else if(headerslist[0].contains("POST"))
        type = "POST";
    else if(type.isEmpty())
        type = "UNDIFINED";
    jsonObj["Type"] = type;



    //Создание пары ключ-значение
    //
    //Создание пары ключ-значение
    for(int i = 1; i < headerslist.length(); i++)
    {
        int k = headerslist[i].indexOf(':');
        name = headerslist[i].left(k);
        arg = headerslist[i].right(headerslist[i].length() - k - 2);
        jsonObj[name] = arg;
    }


    if (type == "POST")
        jsonObj["Data"] = content;

    //Сохранение в JSON
    //
    //Сохранение в JSON
    QJsonDocument jsonDoc(jsonObj);
    QFile saveJson("./Requests/Input/" + ip.mid(7) + ":" + QString("%1").arg(port) + ":" + type + ":" +  QDateTime::currentDateTime().toString("dd.MM.yyyy").toUtf8().data() + ":" +  QDateTime::currentDateTime().toString("hh.mm.ss").toUtf8().data() + ".json");
    saveJson.open(QIODevice::WriteOnly);
    saveJson.write(jsonDoc.toJson());
    saveJson.close();
}

void Requests::CreateFile(QString name, QString content)
{
    QFile file("./Files/" + name);
    file.open(QIODevice::WriteOnly);
    file.write(content.toStdString().c_str());
    file.close();
}

QString Requests::FileList()
{
    QDir dir;
    dir.cd("./Files/");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    QString outstring;
    for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            outstring.append(fileInfo.fileName());
            outstring.append("\t");
    }
    return outstring;
}

QString Requests::Register(QString username, QString password, QString ip, quint16 port)
{
    if (QFile::exists("./Users/" + username))
            return "User already exist. Take other name\n";
    QFile file("./Users/" + username);
    file.open(QIODevice::WriteOnly);
    file.write(("UserIp: " + ip.mid(7) + "\nUser Port: " + QString("%1").arg(port) + "\n" + "User password: " + password).toStdString().c_str());
    file.close();
    return "New user completely created! Go SignIn!\n";
}

bool Requests::SingIn(QString username, QString password)
{
    if (!QFile::exists("./Users/" + username))
            return false;
    QFile file("./Users/" + username);
    file.open(QIODevice::ReadOnly);
    QString ident(file.readAll());
    file.close();
    if (ident.contains(password))
        return true;
    else
        return false;
}

QString Requests::TakeFile(QString name)
{
    QFile file("./Files/" + name);
    file.open(QIODevice::ReadOnly);
    QString output(file.readAll());
    file.close();
    return output;
}



Requests::Requests(){

}



QStringList Requests::readJSON(QString name)
{
    QStringList output;
    QFile loadJson("./Requests/Output/" + name + ".json");
    loadJson.open(QIODevice::ReadOnly);
    QByteArray loadData = loadJson.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(loadData));
    QJsonObject jsonObj = jsonDoc.object();
    output << jsonObj["Header"].toString() << jsonObj["Data"].toString();
    loadJson.close();
    return output;
}
