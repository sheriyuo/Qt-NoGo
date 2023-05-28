#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QFile>
#include <QTcpSocket>
#include <QHostAddress>
#include <QRandomGenerator>
#include <QHostInfo>
#include <QDir>
#include <QStandardPaths>

/*
*   @file: logger.h
*   @brief: 声明 Logger 类，
*           在 C:/Users/<your_name>/AppData/Local/NOGO/log.txt 下输出运行日志
*
*           构造方法
*           > Logger(Level level, const QString &message, QObject* parent = nullptr);
*           > eum class Level {Debug = 1000, Info, Warning, Error}
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*/

enum class Level {
    Debug = 1000,
    Info,
    Warning,
    Error
};

class Logger : public QObject
{
    Q_OBJECT

public:
    Logger(QObject* parent = nullptr);
    ~Logger();

    void log(Level level, const QString &message);

private:
    QString token;
    QFile m_logFile;
    bool openLogFile();
};

#endif // LOGGER_H
