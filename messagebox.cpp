#include "messagebox.h"

MessageBox::MessageBox(QString text, int tim, QWidget *parent):
    QLabel(text, parent)
{
    timer = new QTimer(this);
    timer->start(tim); // ms
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &MessageBox::timeUpClose);
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

void MessageBox::timeUpClose()
{
    this->close();
}

void MessageBox::mousePressEvent(QMouseEvent *event)
{
    // qDebug() << "qwq\n";
    emit mousePress();
}
