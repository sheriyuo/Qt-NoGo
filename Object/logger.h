#ifndef LOGGER_H
#define LOGGER_H


#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include<QFile>
#include <QTcpSocket>
#include <QHostAddress>
#include <QHostInfo>
#include <string>
#include <QDir>
//#include "judge.h"
//#include "Network/networkdata.h"



class Logger : public QObject
{
    Q_OBJECT

public:
    enum class Level {
        Debug,
        Info,
        Warning,
        Error
    };

    //Logger();

    void log(Level level, const QString &message);

    bool openLogFile();

private:
    QFile m_logFile;
};






























#endif // LOGGER_H
