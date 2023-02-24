#ifndef BOT_H
#define BOT_H

#include <QObject>
#include <QDebug>
#include "judge.h"

class Bot : public QObject
{
    Q_OBJECT

public:
    Bot (QObject *parent = nullptr);
    ~Bot();
    void makeRandomMove();

    Judge *judge;

public slots:
    void makeFirstMove(int player);

private:
};

#endif // BOT_H
