/**
 * ipchub.h
 * Erick Veil
 * 2021-03-01
 * Copyright 2021 Erick Veil
 */

#ifndef IPCHUB_H
#define IPCHUB_H

#include <functional>

#include "../constants.h"
#include "locallisten.h"
#include "staticlogger.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
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
    QJsonValue _getReqiredJsonValue(QJsonDocument doc, QString key);


    void _cbProcessSetVar(QByteArray msg);
    QByteArray _cbAckGetVar(QByteArray key);
};

#endif // IPCHUB_H
