// Copyright 2021 Erick Veil

#include "ipchub.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    IpcHub hub;
    hub.start();

    return a.exec();
}
