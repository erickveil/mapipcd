/**
 * ipcclient.h
 * Erick Veil
 * 2021-03-02
 * Copyright 2021 Erick Veil
 */
#ifndef IPCCLIENT_H
#define IPCCLIENT_H

#include "../constants.h"
#include "localsocketclient.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QTextStream>


/**
 * @brief The IpcClient class
 * Initializes and utilizes local socket clients to communicate with mapipcd.
 * Provides an interface for getting and setting values.
 */
class IpcClient
{
    QString _getPort = MapIpcConst::GET_NAME;
    QString _setPort = MapIpcConst::SET_NAME;

    LocalSocketClient _setClient;
    LocalSocketClient _getClient;

    bool _isInit = false;

public:
    IpcClient();

    /**
     * @brief init
     * Should call this before using the object.
     * Initializes clients with CBs and names.
     */
    void init();

    /**
     * @brief setValue
     * Sends a request to the daemon to set value at key.
     * Exits quietly after successful completion.
     * Will init the object if not done already.
     * @param key
     * @param value
     */
    void setValue(QString key, QJsonValue value);

    /**
     * @brief getValue
     * Sends a request to the daemon for the value at key. This value is
     * output to stdout before the program exits.
     * Will init the object if not done already.
     * @param key
     */
    void getValue(QString key);

    /**
     * @brief setSetPort
     * Must be called before init
     * Otherwise constants.h define the default values.
     * @param setPortName
     */
    void setSetPort(QString setPortName);

    /**
     * @brief setGetPort
     * Must be called before init
     * Otherwise constants.h define the default values.
     * @param getPortName
     */
    void setGetPort(QString getPortName);

private:
    void _initSetClient();
    void _initGetClient();
    QByteArray _createGetMsg(QString key);
    QByteArray _createSetMsg(QString key, QJsonValue value);
    void _parseGetResponse(QByteArray msg);
    void _printGetResponse(QJsonValue val);
    void _errorCalback(QLocalSocket::LocalSocketError errnum, QString errStr);
};

#endif // IPCCLIENT_H
