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
*
*           constructor:
*           > MessageBox(QWidget *parent = nullptr)
*
*           methods:
*           > void set(QString text, int tim, bool pendingClos)
*             > text : 显示的文字
*             > tim : 超时自动关闭，设置为 0 时， 必须要手动点击消息框才可关闭
*             > pendingClos : 强制 5s 内不能被关闭
*           > bool clickToClose()
*             > 模拟鼠标点击关闭，受 pendingClose 影响
*           > void timeUpClose()
*             > 超时关闭，不受 pendingClose 影响
*
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*

*/

class MessageBox : public QLabel
{
    Q_OBJECT

public:
    MessageBox (QWidget *parent = nullptr);
    ~MessageBox();

signals:

public slots:
    void set(QString text, int tim, bool pendingClos);
    bool clickToClose();
    void timeUpClose();

private:
    QTimer *timer;
    time_t baseTime;
    void mousePressEvent(QMouseEvent *event) override;
    bool pendingClose;
};

#endif // MESSAGEBOX_H
