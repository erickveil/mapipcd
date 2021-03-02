/**
 * commandlineoptions.h
 * Erick Veil
 * 2021-03-02
 * Copyright 2021 Erick Veil
 */
#ifndef COMMANDLINEOPTIONS_H
#define COMMANDLINEOPTIONS_H

#include <QByteArray>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QTextStream>

/**
 * @brief The CommandLineOptions class
 * Handles the command line options provided when running the program.
 */
class CommandLineOptions
{
    bool _isGet = false;
    bool _isSet = false;
    QString _key;
    QString _value;

public:
    CommandLineOptions();

    enum CommandFunction { CMD_INVALID, CMD_GET, CMD_SET };

    void init(QCoreApplication *a);

    CommandFunction getCommand();
    QString getKey();
    QString getValue();
};

#endif // COMMANDLINEOPTIONS_H
