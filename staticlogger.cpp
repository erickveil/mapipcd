// Copyright 2021 Erick Veil

#include "staticlogger.h"


StaticLogger::StaticLogger()
{

}

void StaticLogger::logDebug(const char *method, int line, QString msg)
{
    writeDebug("DEBUG", method, line, msg);
}

void StaticLogger::logInfo(const char *method, int line, QString msg)
{
    writeLog("INFO", method, line, msg);
}

void StaticLogger::logWarn(const char *method, int line, QString msg)
{
    writeLog("WARN", method, line, msg);
}

void StaticLogger::writeLog(QString level, const char *method, int line,
                            QString msg)
{
    msg = sanitize(msg);
    QString record = createRecord(level, method, line, msg);
    QTextStream output( stdout );
    output << record;
}

void StaticLogger::singleReport(QString filepath, QString msg)
{
    QFile file(filepath);
    bool isOpen = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!isOpen) {
        msg = "Could not open file [[" + filepath + "]] for report: " + msg;
        logWarn(__PRETTY_FUNCTION__, __LINE__, msg);
        return;
    }
    QTextStream out(&file);

    Qt::DateFormat format = Qt::ISODateWithMs;
    QString timestamp = QDateTime::currentDateTime().toString(format);
    msg = filepath + "\n" + timestamp + "\n\n" + msg + "\n\n";

    out << msg;

    if (file.isOpen()) { file.close(); }
}

void StaticLogger::writeDebug(QString level, const char *method, int line,
                            QString msg)
{
    QString record = createRecord(level, method, line, msg);
    qDebug() << record;
}

QString StaticLogger::createRecord(QString level, const char *method, int line,
                                   QString msg)
{
    Qt::DateFormat format = Qt::ISODateWithMs;
    QString timestamp = QDateTime::currentDateTime().toString(format);
    QString delim = " | ";
    return level + delim + timestamp + delim + QString(method)
            + delim + QString::number(line) + delim + msg + "\n";
}

QString StaticLogger::sanitize(QString msg)
{
    QString newStr;
    const int ASCII_CARRIAGE_RETURN = 13;
    for (int c=0; c < msg.count(); ++c)
    {
        if (msg[c] == ASCII_CARRIAGE_RETURN)
        {
            newStr += " ";
            continue;
        }
        newStr += msg[c];
    }
    return newStr;
}

