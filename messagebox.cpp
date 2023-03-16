#include "messagebox.h"

/*
 * 关闭方法有手动点击消息框和超时关闭
 * tim : 超时设置
 * 设置为 0 时， 必须要手动点击消息框才可关闭
*/
MessageBox::MessageBox(QString text, int tim, QWidget *parent):
    QLabel(text, parent)
{
    if(tim > 0){
        timer = new QTimer(this);
        timer->start(tim); // ms
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, this, &MessageBox::timeUpClose);
    }
    connect(this, &MessageBox::mousePress, this, &MessageBox::timeUpClose);

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
}

MessageBox::~MessageBox()
{
    delete this;
}

void MessageBox::mousePressEvent(QMouseEvent *event)
{
    emit mousePress();
}

void MessageBox::timeUpClose()
{
    this->close();
}
