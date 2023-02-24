#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QObject>
#include <QLabel>
#include <QTimer>
#include "judge.h"

class MessageBox : public QLabel
{
    Q_OBJECT

public:
    MessageBox (QString text, int tim, QWidget *parent = nullptr);
    ~MessageBox();

public slots:
    void timeUpClose();

private:
    QTimer *timer;
};

#endif // MESSAGEBOX_H
