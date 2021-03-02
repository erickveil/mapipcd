// Copyright 2021 Erick Veil

#include "ipchub.h"

IpcHub::IpcHub()
{

}

void IpcHub::init()
{
    if (_isInit) { return; }
    _initSetVarListener();
    _initGetVarListener();
    _isInit = true;
}

void IpcHub::start()
{
    if (!_isInit) { init(); }
    _setVarListener.start();
    _getVarListener.start();
}

void IpcHub::_initSetVarListener()
{
    _setVarListener.Name = MapIpcConst::SET_NAME;
    _setVarListener.ParseCallback = [&] (QByteArray msg) {
        _cbProcessSetVar(msg);
    };
    _setVarListener.init();
}

void IpcHub::_initGetVarListener()
{
    _getVarListener.Name = MapIpcConst::GET_NAME;
    _getVarListener.AckCallback = [&] (QByteArray msg) {
        return _cbAckGetVar(msg);
    };
    _getVarListener.init();
}

QJsonValue IpcHub::_getReqiredJsonValue(QJsonDocument doc, QString key)
{
    QJsonObject root = doc.object();
    if (root.contains(key)) { return root.value(key); }
    LOG_WARN("JSON Breaks protocol, missing required key '" + key + "': " +
             doc.toJson());
    return QJsonValue();
}

void IpcHub::_cbProcessSetVar(QByteArray msg)
{
    auto doc = QJsonDocument::fromJson(msg);
    bool isBadJson = doc.isNull() || !doc.isObject();
    if (isBadJson) {
        LOG_WARN("Malformed JSON: " + msg);
        return;
    }
    QJsonValue jKey = _getReqiredJsonValue(doc, "key");
    QJsonValue jVal = _getReqiredJsonValue(doc, "value");
    bool isBreaksProtocol = jKey.isNull();
    if (isBreaksProtocol) { return; }
    QString key = jKey.toString();

    bool isClearCmd = jVal.isNull();
    if (isClearCmd) { _valStore.remove(key); }
    else { _valStore[key] = jVal; }
}

QByteArray IpcHub::_cbAckGetVar(QByteArray key)
{
    QJsonDocument doc;
    QJsonObject root;
    root["value"] = QJsonValue();

    // return NULL value if requested key does not exist
    if (!_valStore.contains(key)) {
        doc.setObject(root);
        return doc.toJson();
    }

    // otherwise return QJsonValue that's saved
    root["value"] = _valStore.value(key);
    doc.setObject(root);
    return doc.toJson();
}
