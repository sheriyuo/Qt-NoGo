#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QObject>
#include <QLabel>
#include <QTimer>
#include <QMouseEvent>
#include "judge.h"

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
