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

//定义此宏是为了判断头格式事都是json类型还是x-www-form类型
#define JSON

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
signals:

public slots:
private:

   QJsonObject m_jsonObject;
   QByteArray m_token; //目前token没有用到，暂时先放着
};

#endif // NETWORKREQUEST_H
