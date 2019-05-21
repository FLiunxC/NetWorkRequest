#include "NetworkRequest.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QUrl>
#include <QDebug>
#include <QNetworkInterface>
#include <QXmlStreamWriter>


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

void NetworkRequest::addSoapKeyValue(const QString &key, const QString &value)
{
    m_SoapMap.insert(key, value);
}

void NetworkRequest::setSoapMethodServer(const QString &methodName, const QString &serverNameSpaceUrl)
{
    m_SoapMethonName = methodName;
    m_SoapServerNameSpaceUrl = serverNameSpaceUrl;
}

QNetworkReply *NetworkRequest::doPostRequest(const QString &targetUrl,  QNetworkAccessManager *networkManager)
{
    QNetworkRequest request;

    QString url = targetUrl;
//    if(!url.endsWith("?")) {
//        url = url +"?";
//    }
    if(url.startsWith("https"))
    {
        m_httpsRequest = true;
    }
    else {
        m_httpsRequest = false;
    }
    qInfo()<<"post URL = "<<url;
    request.setUrl(QUrl(url));

    return this->doPostRequest(request, networkManager);
}

QNetworkReply* NetworkRequest::doPostRequest(const QNetworkRequest &request,  QNetworkAccessManager *networkManager)
{
    QByteArray PostData;
    QNetworkRequest req = request;

    if(m_httpsRequest)
    {
        QSslConfiguration config;
        QSslConfiguration conf = request.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setProtocol(QSsl::TlsV1SslV3);
        req.setSslConfiguration(conf);
    }
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

    if(url.startsWith("https"))
    {
        m_httpsRequest = true;
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
    if(m_httpsRequest)
    {
        QSslConfiguration config;
        QSslConfiguration conf = request.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setProtocol(QSsl::TlsV1SslV3);
        request2.setSslConfiguration(conf);
    }
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

QString NetworkRequest::getIpv4Addre()
{

        QList<QHostAddress> list =QNetworkInterface::allAddresses();
        foreach (QHostAddress address, list)
        {
           if(address.protocol() ==QAbstractSocket::IPv4Protocol)
               //我们使用IPv4地址
               return address.toString();
        }

        return "0";

}

QNetworkReply *NetworkRequest::doSoapRequest(const QString &targetUrl, QNetworkAccessManager *networkManager)
{
    if(targetUrl.startsWith("https"))
    {
        m_httpsRequest = true;
    }
    QNetworkRequest networkRequest;
    // 发送https请求前准备工作;
    if(m_httpsRequest)
    {
        QSslConfiguration config;
        QSslConfiguration conf = networkRequest.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setProtocol(QSsl::TlsV1SslV3);
        networkRequest.setSslConfiguration(conf);
    }

    networkRequest.setUrl(QUrl(targetUrl));

    return this->doSoapRequest(networkRequest, networkManager);
}

QNetworkReply *NetworkRequest::doSoapRequest(const QNetworkRequest &request, QNetworkAccessManager *networkManager)
{
    QNetworkRequest networkRequestBak = request;
    networkRequestBak.setHeader(QNetworkRequest::ContentTypeHeader,("text/xml;charst=utf-8"));
    QByteArray soapXml = writeSoapXML();

    return networkManager->post(networkRequestBak, soapXml);
}

QByteArray NetworkRequest::writeSoapXML()
{
    QByteArray byteArray;
    QXmlStreamWriter xmlWrite(&byteArray);

    QString namespaceUri = XML_SOAP_HTTP;
    QString prefixStr = "soapenv";
    QString prefixStrWeb = "web";

    //自动格式化
    xmlWrite.setAutoFormatting(true);
    xmlWrite.writeStartDocument("1.0", true);
    xmlWrite.writeNamespace(namespaceUri,prefixStr);

    //前缀XML声明
    xmlWrite.writeStartElement(namespaceUri,"Envelope");
    xmlWrite.writeNamespace(m_SoapServerNameSpaceUrl, prefixStrWeb);
    xmlWrite.writeStartElement(namespaceUri, "Header");
    xmlWrite.writeStartElement(namespaceUri, "Body");
    xmlWrite.writeStartElement(m_SoapServerNameSpaceUrl, m_SoapMethonName);

    QStringList keyString = m_SoapMap.keys();

    //循环写入参数到xml
    for(auto key : keyString)
    {
        //参数项的声明
        xmlWrite.writeComment("Optional");
        //写入参数文本
        xmlWrite.writeTextElement(key, m_SoapMap.value(key));
    }

    xmlWrite.writeEndElement(); //结束m_SoapMethonName方法名称标签
    xmlWrite.writeEndElement(); //结束Header标签
    xmlWrite.writeEndElement();  //结束Body标签
    xmlWrite.writeEndElement();  //结束Envelope标签
    xmlWrite.writeEndDocument(); //结束文档

    qInfo()<<"生成的Soap Xml"<<byteArray;
    return byteArray;
}
