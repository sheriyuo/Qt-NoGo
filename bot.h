#ifndef BOT_H
#define BOT_H

#include <QObject>
#include <QDebug>
#include "judge.h"

#define CHESSBOARD_SIZE judge->CHESSBOARD_SIZE
#define GRID_THICKNESS judge->GRID_THICKNESS
#define SQUARE_LEN judge->SQUARE_LEN
#define LEFT_UP judge->LEFT_UP
#define RIGHT_UP judge->RIGHT_UP

class Bot : public QObject
{
    Q_OBJECT

public:
    Bot (QObject *parent = nullptr);
    ~Bot();

    Judge *judge;

signals:
    void timeout();

public slots:
    void makeFirstMove(int player);
    void makeAMove();

private:
    void makeRandomMove();
};

#endif // BOT_H
