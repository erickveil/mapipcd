// Copyright 2021 Erick Veil

#include "ipcclient.h"

IpcClient::IpcClient()
{

}

void IpcClient::init()
{
    _initGetClient();
    _initSetClient();
}

void IpcClient::setValue(QString key, QJsonValue value)
{

}

QJsonValue IpcClient::getValue(QString key)
{

}

void IpcClient::_initSetClient()
{
    auto parseCb = [&] (QByteArray msg) {
        exit(EXIT_SUCCESS);
    };
    _setClient.ParseResponse = parseCb;
}

void IpcClient::_initGetClient()
{

}
