#ifndef GLOBALNETWORKACCESSMANAGER_H
#define GLOBALNETWORKACCESSMANAGER_H


/**
* @brief: 全局维护一个QNetworkAccessManager，单利访问
* @author: Fu_Lin
* @date:  2019-05-10
* @description: 全局维护一个网络类，防止多处new，该类太累赘，维护一个即可
*/

#include <QObject>
#include <QNetworkAccessManager>

class GlobalNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit GlobalNetworkAccessManager(QNetworkAccessManager *parent = nullptr);

    inline GlobalNetworkAccessManager * getNetworkAccessManager()
    {
        return this;
    }

signals:

public slots:
};

#endif // GLOBALNETWORKACCESSMANAGER_H
