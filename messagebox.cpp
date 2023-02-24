#include "messagebox.h"

MessageBox::MessageBox(QString text, int tim, QWidget *parent):
    QLabel(text, parent)
{
    timer = new QTimer(this);
    timer->start(tim); // ms
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &MessageBox::timeUpClose);

    this->setAlignment(Qt::AlignCenter);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setGeometry(WINDOW_WIDTH / 2 - WINDOW_HEIGHT / 6, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 12,
                      WINDOW_HEIGHT / 3, WINDOW_HEIGHT / 6);
    this->setWindowTitle(QString("NoGo Message"));
    this->setText(text);
    this->setStyleSheet("font: 24px 'Arial Rounded MT Bold';"
                        "background-color: rgb(234, 241, 214);"
                        "border-radius: 20px;"
                        "color: rgb(96, 153, 102);");
}

MessageBox::~MessageBox()
{
    /*empty*/
}

void MessageBox::timeUpClose()
{
    this->close();
    delete this;
}
