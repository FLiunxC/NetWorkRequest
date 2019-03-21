#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

#include <QObject>
#include <QNetworkConfigurationManager>
//#include "Singleton.h"

class NetworkMonitor : public QObject
{
    Q_OBJECT
public:
    explicit NetworkMonitor(QObject *parent = nullptr);

    /**
     * @brief isNetWorkOnline 手动调用是否有网
     * @return  true--有网， false -- 无网
     */
    bool isNetWorkOnline();
signals:
    void sigNetworkStateChanged(bool isOnline);
public slots:

private:
    QNetworkConfigurationManager * m_networkConfigMgr = nullptr;
};

#endif // NETWORKMONITOR_H
