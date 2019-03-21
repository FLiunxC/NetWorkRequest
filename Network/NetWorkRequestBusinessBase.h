#ifndef NETWORKREQUESTBUSINESSBASE_H
#define NETWORKREQUESTBUSINESSBASE_H

#include <QObject>
#include "NetworkRequest.h"


/**
* @brief: 网络业务请求的基类
* @author: Fu_Lin
* @date: 时间日期
* @description: 直接和封装好的网络请求类打交道
*/

class jsonParseBase : public QObject
{
    Q_OBJECT

public:
    explicit jsonParseBase(QObject *parent = nullptr);

    virtual ~jsonParseBase()
    {
        qInfo()<<"jsonParseBase"<<"函数进入";
    }

public:
    virtual void  jsonParse(QJsonDocument  document) = 0;
};


class NetWorkRequestBusinessBase : public jsonParseBase
{
    Q_OBJECT
public:
    explicit NetWorkRequestBusinessBase(jsonParseBase *parent = nullptr);

    virtual  ~NetWorkRequestBusinessBase();


protected:
    virtual void jsonParse(QJsonDocument  document) = 0;



protected:
    void dopostRequest(NetworkRequest *request, QString target, QNetworkAccessManager * network, jsonParseBase * PjsonParse , bool isSynchronous = false);

private:
    void cleanRelpy();
signals:

public slots:

    void doFinish_slot();
    void doError_slot(QNetworkReply::NetworkError);
private:


    QNetworkReply * m_Reply = nullptr;
    jsonParseBase * m_JsonParseBase = nullptr;

};

#endif // NETWORKREQUESTBUSINESSBASE_H
