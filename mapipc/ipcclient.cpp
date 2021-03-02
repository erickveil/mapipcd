// Copyright 2021 Erick Veil

#include "ipcclient.h"

IpcClient::IpcClient()
{

}

void IpcClient::init()
{
    if (_isInit) { return; }
    _initGetClient();
    _initSetClient();
    _isInit = true;
}

void IpcClient::setValue(QString key, QJsonValue value)
{
    if (!_isInit) { init(); }
    QByteArray msg = _createSetMsg(key, value);
    _setClient.send(msg);
}

void IpcClient::getValue(QString key)
{
    if (!_isInit) { init(); }
    QByteArray msg = _createGetMsg(key);
    _getClient.send(msg);
}

void IpcClient::_initSetClient()
{
    auto parseCb = [&] (QByteArray msg) { exit(EXIT_SUCCESS); };
    _setClient.ParseResponse = parseCb;
    _setClient.Name = MapIpcConst::SET_NAME;
    _setClient.init();
}

void IpcClient::_initGetClient()
{
    auto parseCb = [&] (QByteArray msg) { _parseGetResponse(msg); };
    _getClient.Name = MapIpcConst::GET_NAME;
    _getClient.init();
}

QByteArray IpcClient::_createGetMsg(QString key)
{
    return key;
}

QByteArray IpcClient::_createSetMsg(QString key, QJsonValue value)
{
    QJsonDocument doc;
    QJsonObject obj;
    obj[key] = value;
    doc.setObject(obj);
    return doc.toJson();
}

void IpcClient::_parseGetResponse(QByteArray msg)
{
    QTextStream errorOut( stderr );
    auto doc = QJsonDocument::fromJson(msg);
    if (doc.isNull()) {
        errorOut << "Response from daemon is malformed: " << msg;
        errorOut.flush();
        exit(EXIT_FAILURE);
    }
    if (!doc.isObject()) {
        errorOut << "Response from daemon does not follow expected protocol: "
                 << msg;
        errorOut.flush();
        exit(EXIT_FAILURE);
    }
    QJsonObject obj = doc.object();
    if (!obj.contains("value")) {
        errorOut << "Response from daemon does not contain value member: "
                 << msg;
        errorOut.flush();
        exit(EXIT_FAILURE);
    }
    QJsonValue val = obj.value("value");

    // a library would differ at this point and instead return the QJsonValue
    // from a method or make it avialable somehow.
    _printGetResponse(val);
}

void IpcClient::_printGetResponse(QJsonValue val)
{
    QTextStream output( stdout );
    output << val.toString();
    output.flush();
    exit(EXIT_SUCCESS);
}
