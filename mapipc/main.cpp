#include <QCoreApplication>
#include <QJsonValue>
#include <QTextStream>

#include "commandlineoptions.h"
#include "ipcclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("mapipc");
    QCoreApplication::setApplicationName("1.0");

    CommandLineOptions ops;
    ops.init(&a);

    IpcClient client;
    if (ops.isUsingAltPort()) {
        client.setGetPort(ops.getAltPortValue());
        client.setSetPort(ops.getAltPortValue());
    }
    client.init();

    bool isNoKey = (ops.getKey() == "") &&
            (ops.getCommand() == CommandLineOptions::CMD_GET ||
             ops.getCommand() == CommandLineOptions::CMD_SET);
    if (isNoKey) {
        QTextStream errout( stderr );
        errout << "Key (-k) argument must be provided for command. Run "
                  "`mmapipc -h` for more help.\n";
        errout.flush();
        exit(EXIT_FAILURE);
    }

    if (ops.getCommand() == CommandLineOptions::CMD_GET) {
        client.getValue(ops.getKey());
    }
    else if (ops.getCommand() == CommandLineOptions::CMD_SET) {
        QJsonValue value;
        bool isClearValueCmd = ops.getValue() == "";
        if (!isClearValueCmd) { value = ops.getValue(); }
        // else the null value is the command for the map to clear the key.
        client.setValue(ops.getKey(), value);
    }

    return a.exec();
}
