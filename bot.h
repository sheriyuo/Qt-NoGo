#ifndef BOT_H
#define BOT_H

#include <QObject>
#include <QDebug>
#include "judge.h"

class Bot : public QObject
{
    Q_OBJECT

public:
    Bot (Judge *j, QObject *parent = nullptr);
    ~Bot();

signals:
    void timeout();

public slots:
    void makeFirstMove(int player);
    void makeAMove();

private:
    void makeRandomMove();

    Judge *judge;
};

#endif // BOT_H
