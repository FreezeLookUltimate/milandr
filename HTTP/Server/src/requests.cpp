#include "requests.h"


Requests::Requests(QStringList headers, QString str)
{

    headerslist = headers;
    content = str;
    for(int i = 0; i < headerslist.length(); i++){
        qDebug() <<headerslist[i];
    }
    qDebug() << "POST DATA: " <<str;
    qDebug() << "POST DATA LENGTH: "<< content.length();
}
