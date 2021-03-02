/**
 * staticlogger.h
 * Erick Veil
 * 2018-10-02
 *
 * Copyright 2018 Erick Veil
 *
 * Changelog:
 *
 * 2021-03-01
 * EV
 * Modified for mapipcd
 */
#ifndef STATICLOGGER_H
#define STATICLOGGER_H

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>

/**
 * @brief The StaticLogger class
 * A simple logger that writes to stdout
 */

#define LOG_DEBUG(a) (StaticLogger::logDebug(__PRETTY_FUNCTION__, __LINE__, a))
#define LOG_INFO(a) (StaticLogger::logInfo(__PRETTY_FUNCTION__, __LINE__, a))
#define LOG_WARN(a) (StaticLogger::logWarn(__PRETTY_FUNCTION__, __LINE__, a))
#define LOG_ERROR(a) (StaticLogger::writeLog("ERROR", __PRETTY_FUNCTION__, __LINE__, a))
#define LOG_HEADER(a) (StaticLogger::writeLog("HEADER", __PRETTY_FUNCTION__, __LINE__, a))

/**
 * @brief The StaticLogger class
 * A simple logger that writes to stdout
 * Use the macros. They make life easiest.
 */
class StaticLogger
{
public:
    StaticLogger();
    static void logDebug(const char* method, int line, QString msg);
    static void logInfo(const char* method, int line, QString msg);
    static void logWarn(const char* method, int line, QString msg);
    static void writeLog(QString level, const char* method, int line,
                         QString msg);
    static void singleReport(QString filepath, QString msg);
private:
    static void writeDebug(QString level, const char* method, int line,
                           QString msg);
    static QString createRecord(QString level, const char* method, int line,
                                QString msg);
    static QString sanitize(QString msg);
};


#endif // STATICLOGGER_H
