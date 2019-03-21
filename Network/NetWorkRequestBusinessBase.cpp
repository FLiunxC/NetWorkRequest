#include "NetWorkRequestBusinessBase.h"
#include "QEventLoop"
#include "qdebug.h"
#include <QJsonParseError>

NetWorkRequestBusinessBase::NetWorkRequestBusinessBase(jsonParseBase *parent) : jsonParseBase(parent)
{

}

NetWorkRequestBusinessBase::~NetWorkRequestBusinessBase()
{

    qInfo()<<"NetWorkRequestBusinessBase 析构函数进入";

    cleanRelpy();

    if(m_JsonParseBase)
    {
        m_JsonParseBase = nullptr;
    }
}


void NetWorkRequestBusinessBase::dopostRequest(NetworkRequest *request, QString target, QNetworkAccessManager * network,  jsonParseBase *PjsonParse, bool isSynchronous)
{

    cleanRelpy();
    m_JsonParseBase = PjsonParse;

    m_Reply = request->doPostRequest(target,network);
    QObject::connect(m_Reply, &QNetworkReply::finished, this, &NetWorkRequestBusinessBase::doFinish_slot);
    QObject::connect(m_Reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(doError_slot(QNetworkReply::NetworkError)));

    QEventLoop loop;

    if(isSynchronous) {
        QObject::connect(m_Reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
    }

}

void NetWorkRequestBusinessBase::cleanRelpy()
{
    if(m_Reply) {
        m_Reply->abort();
        delete  m_Reply;
        m_Reply = nullptr;
    }
}

void NetWorkRequestBusinessBase::doFinish_slot()
{
    QByteArray jsonData = m_Reply->readAll();


    qInfo() << "url = " << m_Reply->url().toString();
    qInfo() << "jsonData"<<jsonData;

    QObject::disconnect(m_Reply, &QNetworkReply::finished, this, &NetWorkRequestBusinessBase::doFinish_slot);

    cleanRelpy();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(jsonData, &jsonError);

    if(document.isNull() || (jsonError.error != QJsonParseError::NoError))
    {
        qCritical()<<"jsonError:"<<jsonError.error;
        return;
    }


    if(m_JsonParseBase)
    {
          m_JsonParseBase->jsonParse(document);
    }

}

void NetWorkRequestBusinessBase::doError_slot(QNetworkReply::NetworkError error)
{
    qInfo()<<"error = "<<error;
}

jsonParseBase::jsonParseBase(QObject *parent): QObject(parent)
{

}
