#ifndef NETWORKREQUEST_H
#define NETWORKREQUEST_H

/**
* @brief: 封装好的网络请求模块
* @author: Fu_Lin
* @date:
* @description: 用于简化使用者使用http请求访问，使用者只需要关心返回的QNetworkReply即可
*/

#include <QObject>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QNetworkReply>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QFile>
#include <QNetworkAccessManager>
#include <QMap>
#include <QMultiMap>

//定义此宏是为了判断头格式事都是json类型还是x-www-form类型
#define JSON

//XMLSOAP的httpUrl 名词空间标识符
#define XML_SOAP_HTTP "http://schemas.xmlsoap.org/soap/envelope/"

using  QMapString=QMap<QString, QString>;
using  QMultiMapString = QMultiMap<QString, QString>;


class NetworkRequest : public QObject
{
    Q_OBJECT
public:
    explicit NetworkRequest(QObject *parent = nullptr);

    /**
     * @brief addKeyValue 请求添加的参数字段
     * @param key  参数key
     * @param value 参数值
     */
    void addKeyValue(const QString &key, const QJsonValue &value);
    void addKeyValue(const QString &key, const QStringList & valuelist);

    /**
     * @brief addSoapKeyValue  添加soap需要的key和value
     * @param key 对应的key
     * @param value 对应的value
     */
    void addSoapKeyValue(const QString &key, const QString &value);

    /**
     * @brief setSoapMethodServer 请求soap对应的方法名称和服务名称
     * @param methodName 对应请求的方法
     * @param serverNameSpaceUrl 服务名称一般要去wsdl文档xml中的targetNamespace标签获取
     */
    void setSoapMethodServer(const QString & methodName, const QString & serverNameSpaceUrl);

    /**
     * @brief doPostRequest post请求操作，调用前必须要调用addKeyValue设置好请求参数
     * @param targetUrl 请求的目标URL
     */
    QNetworkReply * doPostRequest(const QString &targetUrl,  QNetworkAccessManager *networkManager);

    /**
     * @brief doPostRequest post重载函数
     * @param request 请求应答的QNetworkRequest，需在外部填写好相关URL后传入
     */
    QNetworkReply * doPostRequest(const QNetworkRequest &request,  QNetworkAccessManager *networkManager);

    /**
    * @brief doGetRequest Get请求操作，调用前必须要调用addKeyValue设置好请求参数
    * @param targetUrl  请求的目标URL
    */
   QNetworkReply * doGetRequest(const QString &targetUrl,  QNetworkAccessManager *networkManager);

   /**
    * @brief doGetRequest Get重载函数
    * @param request 请求应答的QNetworkRequest，需在外部填写好相关URL后传入
    */
   QNetworkReply * doGetRequest(const QNetworkRequest &request, QNetworkAccessManager *networkManager);

   /**
    * @brief UploadFile 上传文件到服务器
    * @param Filepath 文件的路径
    * @return
    */
   QNetworkReply *uploadFile(const QString &Filepath, QString targetUrl,  QNetworkAccessManager &networkManager);

   //下載文件未實現


   /**
    * @brief doSoapRequest Soap协议请求
    * @param targetUrl 请求的目标url
    * @param networkManager 请求的网络管理类
    * @return 返回网络请求应答
    */
   QNetworkReply * doSoapRequest(const QString& targetUrl, QNetworkAccessManager *networkManager);

   /**
    * @brief doSoapRequest 重载Soap请求函数
    * @param request
    * @param networkManager
    * @return
    */
   QNetworkReply *doSoapRequest(const QNetworkRequest &request, QNetworkAccessManager *networkManager);

signals:

public slots:

public:
   /**
    * @brief writeSoapXML  写入soapXml内容
    * @return 返回QByteArray类型Xml数据
    */
   QByteArray writeSoapXML();
private:

   //http请求需要用到的变量
   QJsonObject m_jsonObject;
   QByteArray m_token; //目前token没有用到，暂时先放着

   //SOAP请求需要用到的变量
   QMultiMapString m_SoapMap; //存储soapkey和value的值
   QString m_SoapServerNameSpaceUrl; //存储soap的xml中的targetNamespace标签地址不是请求url
   QString m_SoapMethonName; //存储soap中的方法
};

#endif // NETWORKREQUEST_H
