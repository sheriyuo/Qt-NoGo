#include "Object/logger.h"

Logger::Logger(QObject *parent) :
    QObject(parent)
{
    token = QString::number(QRandomGenerator::global()->bounded(114514));
}
Logger::~Logger()
{
    delete this;
}

void Logger::log(Level level, const QString &message)
{
    QString levelString ;
    switch (level)
    {
    case Level::Debug:
        levelString = "DEBUG";
        break;
    case Level::Info:
        levelString = "INFO";
        break;
    case Level::Warning:
        levelString = "WARNING";
        break;
    case Level::Error:
        levelString = "ERROR";
        break;
    default:
        levelString = "UNKNOWN";
        break;
    }

    QString output = levelString+
                     QString("[%1] %2\n")
                            .arg(QDateTime::currentDateTime().toString(Qt::ISODate))
                            .arg(message);

    // 输出到控制台F
    QTextStream(stdout) << output;
    openLogFile();
    // 输出到文件
    if (m_logFile.isOpen())
    {
        QTextStream(&m_logFile) << output;
        m_logFile.flush();
    }
}

bool Logger::openLogFile()
{
    if (m_logFile.isOpen()) m_logFile.close();

    QString folderPATH, PATH;
    folderPATH += "C:\\Users\\";
    folderPATH += QStandardPaths::writableLocation(QStandardPaths::HomeLocation).section("/",-1,-1);
    folderPATH += "\\AppData\\Local\\NOGO\\";
    qDebug() << folderPATH;
    PATH = folderPATH + "log" + token + ".txt";

    QDir dir;
    if (!dir.exists(folderPATH)) {
        bool success = dir.mkpath(folderPATH);
        if (success) {
            qDebug() << "New folder created successfully!";
        } else {
            qDebug() << "Failed to create new folder!";
        }
    } else {
        qDebug() << "Folder already exists!";
    }

    m_logFile.setFileName(PATH);

    if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        return false;
    }
    return true;
}
