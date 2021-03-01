#ifndef IPCHUB_H
#define IPCHUB_H

#include <functional>

#include "jsonquery.h"
#include "locallisten.h"

#include <QByteArray>
#include <QMap>

class IpcHub
{
    LocalListen _setVarListener;
    LocalListen _getVarListener;

    QMap<QString, QJsonValue> _valStore;

    bool _isInit = false;

public:
    IpcHub();

    void init();
    void start();

private:
    void _initSetVarListener();
    void _initGetVarListener();

    void _cbProcessSetVar(QByteArray msg);
    QByteArray _cbAckGetVar(QByteArray msg);
};

#endif // IPCHUB_H
