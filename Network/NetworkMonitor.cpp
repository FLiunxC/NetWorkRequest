#include "NetworkMonitor.h"

NetworkMonitor::NetworkMonitor(QObject *parent) : QObject(parent)
{
         m_networkConfigMgr = new QNetworkConfigurationManager(this);

         connect(m_networkConfigMgr, &QNetworkConfigurationManager::onlineStateChanged, this, &NetworkMonitor::sigNetworkStateChanged);

}

bool NetworkMonitor::isNetWorkOnline()
{
    if(m_networkConfigMgr == nullptr)
    {
        m_networkConfigMgr = new QNetworkConfigurationManager(this);
    }
    return m_networkConfigMgr->isOnline();
}
