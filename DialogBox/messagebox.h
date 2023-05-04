#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QObject>
#include <QLabel>
#include <QTimer>
#include <QMouseEvent>
#include "Object/judge.h"

/*
*   @file: messagebox.h
*   @brief: 声明 MessageBox 类，
*           在对局中弹出提示，或结算信息
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*/

class MessageBox : public QLabel
{
    Q_OBJECT

public:
    MessageBox (QString text, int tim, QWidget *parent = nullptr);
    ~MessageBox();

signals:
    void mousePress();

public slots:
    void timeUpClose();

private:
    QTimer *timer;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // MESSAGEBOX_H
