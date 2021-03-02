/**
 * ipcclient.h
 * Erick Veil
 * 2021-03-02
 * Copyright 2021 Erick Veil
 */
#ifndef IPCCLIENT_H
#define IPCCLIENT_H

#include "localsocketclient.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>


/**
 * @brief The IpcClient class
 * Initializes and utilizes local socket clients to communicate with mapipcd.
 * Provides an interface for getting and setting values.
 */
class IpcClient
{
    LocalSocketClient _setClient;
    LocalSocketClient _getClient;

    bool _isInit = false;

public:
    IpcClient();

    void init();

    void setValue(QString key, QJsonValue value);
    QJsonValue getValue(QString key);

private:
    void _initSetClient();
    void _initGetClient();
};

#endif // IPCCLIENT_H
