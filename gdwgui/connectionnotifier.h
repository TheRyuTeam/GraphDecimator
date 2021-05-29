#pragma once
#include <QObject>
#include <opcuaclient/UaConnection.h>
#include "util.h"

ELESYOPCUACLIENT_START

class ConnectionNotifier : public QObject, public UaConnectionNotifier
{
    Q_OBJECT
public:
    using Ptr = std::shared_ptr<ConnectionNotifier>;

    ConnectionNotifier() : QObject(nullptr) {}

    virtual ~ConnectionNotifier() {}

    virtual void notifyConnectionStatus(bool isConnected) override
    {
        if(isConnected) {
            emit connected();
        }
        else {
            emit disconnected();
        }
    }

signals:
    void connected();
    void disconnected();
};

ELESYOPCUACLIENT_END
