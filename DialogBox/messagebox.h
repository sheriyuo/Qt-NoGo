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
    MessageBox (QString text, int tim,  bool pendingClose, QWidget *parent = nullptr);
    ~MessageBox();

signals:

public slots:
    bool clickToClose();
    void timeUpClose();

private:
    QTimer *timer;
    time_t baseTime;
    void mousePressEvent(QMouseEvent *event) override;
    bool pendingClose;
};

#endif // MESSAGEBOX_H
