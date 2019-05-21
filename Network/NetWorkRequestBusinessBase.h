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
        qInfo()<<"jsonParseBase"<<"析构函数进入";
    }

public:
    //QByteArray是未做解析的json数据
    virtual void  jsonParse(QByteArray) = 0;
};


class NetWorkRequestBusinessBase : public jsonParseBase
{
    Q_OBJECT
public:
    explicit NetWorkRequestBusinessBase(jsonParseBase *parent = nullptr);

    virtual  ~NetWorkRequestBusinessBase();


protected:
    virtual void jsonParse(QByteArray  document) = 0;



protected:
    /**
     * @brief dopostRequest Http协议Post请求
     * @param request 请求的封装网络类
     * @param target 请求的目标url
     * @param network 请求的网络管理类，外面传入
     * @param PjsonParse 请求的解析Json类(抽象类)
     * @param isSynchronous 是否同步等待请求，默认异步请求
     */
    void dopostRequest(NetworkRequest *request, QString targetUrl, QNetworkAccessManager * network, jsonParseBase * PjsonParse , bool isSynchronous = false);

    /**
     * @brief doSoapRequest Soap协议请求
     * @param request 请求的封装网络类
     * @param targetUrl  请求调用的目标url
     * @param network 请求的网络管理类，外面传入
     * @param PjsonParse 请求的解析Json类(抽象类)
     * @param isSynchronous 是否同步等待请求，默认异步请求
     */
    void doSoapRequest(NetworkRequest *request, QString targetUrl, QNetworkAccessManager * network, jsonParseBase * PjsonParse , bool isSynchronous = false);
private:
    void cleanRelpy();
signals:

public slots:

    void onFinish_slot();
    void doError_slot(QNetworkReply::NetworkError);
private:


    QNetworkReply * m_Reply = nullptr;
    jsonParseBase * m_JsonParseBase = nullptr;

};

#endif // NETWORKREQUESTBUSINESSBASE_H
