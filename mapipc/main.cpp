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
        client.setValue(ops.getKey(), ops.getValue());
    }

    return a.exec();
}
