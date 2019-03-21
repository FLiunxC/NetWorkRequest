#include "NetworkRequest.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QUrl>
#include <QDebug>
#include <QNetworkAccessManager>


NetworkRequest::NetworkRequest(QObject *parent) : QObject(parent)
{

}

void NetworkRequest::addKeyValue(const QString &key, const QJsonValue &value)
{
  //  qInfo()<<"value"<<value.toInt()<<value;
    //构建json对象
    m_jsonObject.insert(key, value);
}

void NetworkRequest::addKeyValue(const QString &key, const QStringList &valuelist)
{
    //构建json数组对象
    QJsonArray jsonArray;

    for(QString value : valuelist)
    {
        jsonArray.append(value);
    }
    m_jsonObject.insert(key, jsonArray);
}

QNetworkReply *NetworkRequest::doPostRequest(const QString &targetUrl,  QNetworkAccessManager *networkManager)
{
    QNetworkRequest request;

    QString url = targetUrl;
//    if(!url.endsWith("?")) {
//        url = url +"?";
//    }
    qInfo()<<"post URL = "<<url;
    request.setUrl(QUrl(url));

    return this->doPostRequest(request, networkManager);
}

QNetworkReply* NetworkRequest::doPostRequest(const QNetworkRequest &request,  QNetworkAccessManager *networkManager)
{
    QByteArray PostData;
    QNetworkRequest req = request;

#ifdef JSON
    req.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
//    req.setRawHeader("Content-Type","application/json");
//    req.setRawHeader("Accept","text/json,*/*;q=0.5");//服务器要求的数据头部
#else
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
#endif

    // 构建 JSON 文档
    QJsonDocument document;
    document.setObject(m_jsonObject);
    PostData = document.toJson(QJsonDocument::Compact);

    QStringList keyList = m_jsonObject.keys();

    qInfo() << "----------------------------------------------------m_jsonObject = " << m_jsonObject;
    //将json清空
    foreach (QString it, keyList) {
        m_jsonObject.remove(it);
    }
    qInfo() << "----------------------------------------------------req = " << req.url() << req.rawHeaderList() << req.rawHeader("Content-Type");
    qInfo() << "----------------------------------------------------post = " <<  PostData;

    return networkManager->post(req, PostData);
}

QNetworkReply *NetworkRequest::doGetRequest(const QString &targetUrl, QNetworkAccessManager *networkManager)
{
    QNetworkRequest request;
    QString url = targetUrl;

    if(!url.endsWith("?")) {
        url = url +"?";
    }

    //get参数进行赋值
    QStringList keyList = m_jsonObject.keys();

    for(int i = 0; i < keyList.length(); i++) {
        QString key = keyList.at(i);
        QJsonValue jsonvalue =  m_jsonObject.value(key);
        QString argument;

        if(jsonvalue.isString()) {
            argument  = key +"="+jsonvalue.toString();
        } else if(jsonvalue.isDouble()) {
            argument  = key +"="+QString::number(jsonvalue.toDouble());
        }

        if(i != keyList.length() - 1) {
            argument += "&";
        }

        url += argument;
    }

    //将json清空
    foreach (QString it, keyList) {
        m_jsonObject.remove(it);
    }

    qInfo() << "Url = " << url;
    request.setUrl(url);

    return  doGetRequest(request, networkManager);
}

QNetworkReply *NetworkRequest::doGetRequest(const QNetworkRequest &request,  QNetworkAccessManager *networkManager)
{
    QNetworkRequest request2 = request;

   // request2.setRawHeader("Content-Type","application/json");
#ifdef JSON
    request2.setRawHeader("Content-Type","application/json");
//    req.setRawHeader("Content-Type","application/json");
//    req.setRawHeader("Accept","text/json,*/*;q=0.5");//服务器要求的数据头部
#else
    request2.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#endif

    QNetworkReply *  reply = networkManager->get(request2);

    return reply;
}

QNetworkReply *NetworkRequest::uploadFile(const QString &Filepath, QString targetUrl,  QNetworkAccessManager &networkManager)
{
    QString path = Filepath;
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%1\"; filename=\"%2\"").arg("file").arg(path));
    QFile *file = new QFile(path);
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);

    file->setParent(multiPart);
    multiPart->append(imagePart);

    QNetworkRequest request;
    request.setUrl(targetUrl);

    QNetworkReply *  reply = networkManager.post(request, multiPart);
    multiPart->setParent(reply);

    return reply;
}
