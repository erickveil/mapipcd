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
    _setVarListener.Name = "mapipc_set";
    _setVarListener.ParseCallback = [&] (QByteArray msg) {
        _cbProcessSetVar(msg);
    };
    _setVarListener.init();
}

void IpcHub::_initGetVarListener()
{
    _getVarListener.Name = "mapipc_get";
    _getVarListener.AckCallback = [&] (QByteArray msg) {
        return _cbAckGetVar(msg);
    };
    _getVarListener.init();
}

void IpcHub::_cbProcessSetVar(QByteArray msg)
{
    auto doc = QJsonDocument::fromBinaryData(msg);
    bool isBadJson = doc.isNull();

}

QByteArray IpcHub::_cbAckGetVar(QByteArray msg)
{

}
