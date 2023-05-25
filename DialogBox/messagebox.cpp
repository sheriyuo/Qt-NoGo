#include "DialogBox/messagebox.h"

MessageBox::MessageBox(QWidget *parent):
    QLabel("", parent)
{
    this->setAlignment(Qt::AlignCenter);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setGeometry(WINDOW_WIDTH / 2 - WINDOW_HEIGHT / 5, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 10,
                      WINDOW_HEIGHT / 2.5, WINDOW_HEIGHT / 5);
    this->setWindowTitle(QString("NoGo Message"));
    this->setStyleSheet("font: 24px 'Arial Rounded MT Bold';"
                        "background-color: rgb(234, 241, 214);"
                        "border-radius: 20px;"
                        "color: rgb(96, 153, 102);");
}

MessageBox::~MessageBox()
{
    qDebug() << "MessageBox Deconstruction";
}

void MessageBox::set(QString text, int tim, bool pendingClos) {
    this->setText(text);
    pendingClose = pendingClos;
    if(tim > 0){
        timer = new QTimer(this);
        timer->start(tim); // ms
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, this, &MessageBox::timeUpClose);
    }

    baseTime = clock();
}

void MessageBox::mousePressEvent(QMouseEvent *event) {clickToClose();}

bool MessageBox::clickToClose()
{
    if(pendingClose && clock() - baseTime <= 3000) return false;
    timeUpClose();
    return true;
}

void MessageBox::timeUpClose() {this->close();}
