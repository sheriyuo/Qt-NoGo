#include "Object/logger.h"
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

        QString output = QString("[%1] %2\n")
                             .arg(QDateTime::currentDateTime().toString(Qt::ISODate))
                             .arg(message)

                ;

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
        if (m_logFile.isOpen()) {
            m_logFile.close();
        }
//        QString PATH,usrname;
//        usrname=QHostInfo::localHostName();
//        PATH+="C:\\Users\\";
//        PATH+=usrname;
//        PATH+="\\AppData\\Local\\NOGO\\Logs\\LOG.txt";
//        qDebug()<<PATH;


        QString folderPath = "C:\\NOGO\\LOGS";
        QDir dir;
        if (!dir.exists(folderPath)) {
            bool success = dir.mkpath(folderPath);
                if (success) {
                    qDebug() << "New folder created successfully!";
                } else {
                    qDebug() << "Failed to create new folder!";
                }
            } else {
                qDebug() << "Folder already exists!";
            }
        QString PATH=folderPath+"\\log.txt";
        qDebug()<<PATH;
        m_logFile.setFileName(PATH);
        if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
            return false;
        }
        return true;
    }



