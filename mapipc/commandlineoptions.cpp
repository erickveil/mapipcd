// Copyright 2021 Erick Veil

#include "commandlineoptions.h"

CommandLineOptions::CommandLineOptions()
{

}

void CommandLineOptions::init(QCoreApplication *a)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Get and Set system global variables for"
                                     "IPC.");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("get",
                                 QCoreApplication::translate(
                                     "main", "Command to get a value. Must "
                                             "provide a key (-k) argument. "
                                             "Example: `mapipc get -k foo`. "
                                             "The -a argument is not used "
                                             "and will be ignored. "
                                             "If the value requested is not"
                                             "set, nothing will be output "
                                             "and the program will exit "
                                             "quietly."));
    parser.addPositionalArgument("set",
                                 QCoreApplication::translate(
                                     "main", "Command to set a value. Must "
                                             "provide both a key (-k) and "
                                             "a value (-a) argument. "
                                             "Example: `mapipc set -a foo "
                                             "-a bar`"));

    QCommandLineOption setKeyOption(
                QStringList() <<
                "k" <<
                "key",
                "Define the key or variable name that the value is to be "
                "saved as. This argument is required for both commands.",
                "key name");
    parser.addOption(setKeyOption);

    QCommandLineOption setValOption(
                QStringList() <<
                "a" <<
                "value",
                "Define the value that is to be saved. Requires the -k option",
                "value to set. If omitted or provided with an empty string "
                "when issuing a `set` command, "
                "the key's value will be cleared.");
    parser.addOption(setValOption);

    parser.process(*a);

    // set config values
    QTextStream errout( stderr );
    QStringList positionalArgs = parser.positionalArguments();
    QString instructions = "Use `mapipc get` or `mapipc set`. Run `mapipc -h` "
                           "for more help.";
    if (positionalArgs.count() > 1) {
        errout << "Too many command arguments. " << instructions << "\n";
        errout.flush();
        exit(EXIT_FAILURE);
    }
    if (positionalArgs.count() == 1) {
        if (positionalArgs[0] == "get") { _isGet = true; }
        else if (positionalArgs[0] == "set") { _isSet = true; }
        else {
            errout << "Unrecognized command arguemnt: " <<
                      positionalArgs[0] <<
                      ". " << instructions << "\n";
            errout.flush();
            exit(EXIT_FAILURE);
        }
    }

    _key = parser.value(setKeyOption);
    _value = parser.value(setValOption);
}

CommandLineOptions::CommandFunction CommandLineOptions::getCommand()
{
    if (_isGet && _isSet) { return CMD_INVALID; }
    if (_isGet) { return CMD_GET; }
    return CMD_SET;
}

QString CommandLineOptions::getKey()
{
    return _key;

}

QString CommandLineOptions::getValue()
{
    return _value;
}
