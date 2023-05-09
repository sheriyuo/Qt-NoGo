#include "DialogBox/messagebox.h"

/*
*   @file: messagebox.h
*   @brief: 声明 MessageBox 类，
*           在对局中弹出提示，或结算信息
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*
*   @constructor: MessageBox(QString text, int tim, bool pendingClos, QWidget *parent)
*                 关闭方法有手动点击消息框和超时关闭
*                 tim : 超时设置
*                 设置为 0 时， 必须要手动点击消息框才可关闭
*                 pendingClos : 强制显示 3s 不能被关闭
*/
MessageBox::MessageBox(QString text, int tim, bool pendingClos, QWidget *parent):
    QLabel(text, parent),
    pendingClose(pendingClos)
{
    if(tim > 0){
        timer = new QTimer(this);
        timer->start(tim); // ms
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, this, &MessageBox::timeUpClose);
    }

    this->setAlignment(Qt::AlignCenter);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setGeometry(WINDOW_WIDTH / 2 - WINDOW_HEIGHT / 5, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 10,
                      WINDOW_HEIGHT / 2.5, WINDOW_HEIGHT / 5);
    this->setWindowTitle(QString("NoGo Message"));
    this->setText(text);
    this->setStyleSheet("font: 24px 'Arial Rounded MT Bold';"
                        "background-color: rgb(234, 241, 214);"
                        "border-radius: 20px;"
                        "color: rgb(96, 153, 102);");

    baseTime = clock();
}

MessageBox::~MessageBox()
{
    delete this;
}

void MessageBox::mousePressEvent(QMouseEvent *event) {clickToClose();}

void MessageBox::clickToClose()
{
    if(pendingClose && clock() - baseTime <= 3000) return;
    timeUpClose();
}

void MessageBox::timeUpClose() {this->close();}
