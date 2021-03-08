// Copyright 2021 Erick Veil

#include "daemonizer.h"

Daemonizer::Daemonizer()
{
}

Daemonizer *Daemonizer::Instance()
{
    if (_instance == nullptr) {
        _instance = new Daemonizer;
    }
    return _instance;
}

void Daemonizer::init()
{
    if (_isInit) { return; }
    signal(SIGINT, Daemonizer::_signalHandler);
    signal(SIGTERM, Daemonizer::_signalHandler);
    signal(SIGHUP, Daemonizer::_signalHandler);

    _signalMonitor.setInterval(250);
    _signalMonitor.setSingleShot(false);
    connect (&_signalMonitor,
             SIGNAL(timeout()),
             this,
             SLOT(_eventCheckSignalFlags()));
    _signalMonitor.start();
    _isInit = true;
}

void Daemonizer::_signalHandler(int signal)
{
    LOG_INFO("Interrupt signal number [" + QString::number(signal) +
             "] received");
    switch (signal) {
    case SIGINT:
        _isSigInt = true;
        break;
    case SIGTERM:
        _isSigTerm = true;
        break;
    case SIGHUP:
        _isSigHup = true;
        break;
    }
}

void Daemonizer::_sigintHandler()
{
    if (SigIntCallback) {
        SigIntCallback();
        return;
    }

    LOG_INFO("SIGINT received.");
    LOG_INFO("The daemon process ended gracefully.");
    QCoreApplication::exit(EXIT_SUCCESS);

}

void Daemonizer::_sigtermHandler()
{
    if (SigTermCallback) {
        SigTermCallback();
        return;
    }
    LOG_INFO("SIGTERM received.");
    LOG_INFO("The daemon process ended gracefully.");
    QCoreApplication::exit(EXIT_SUCCESS);
}

void Daemonizer::_sighupHandler()
{
    if (SigHupCallback) {
        SigHupCallback();
        return;
    }
    LOG_INFO("SIGHUP received.");
    LOG_INFO("This daemon has no configuration - ignoring SIGHUP.");
}

void Daemonizer::_eventCheckSignalFlags()
{
    if (_isSigHup) {
        _isSigHup = false;
        _sighupHandler();
    }

    if (_isSigTerm) {
        _isSigTerm = false;
        _sigtermHandler();
    }

    if (_isSigInt) {
        _isSigInt = false;
        _sigintHandler();
    }
}
