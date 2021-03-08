// Copyright 2021 Erick Veil

#include "daemonizer.h"
#include "ipchub.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto daemonizer = Daemonizer::Instance();
    daemonizer->init();

    IpcHub hub;
    hub.start();

    return a.exec();
}
