#include "judge.h"

Judge::Judge(QObject *parent) :
    QObject(parent)
{
    memset(board, 0, sizeof(board));
}

Judge::~Judge()
{
    /*none*/
}

void Judge::setPlayerRole(int player)
{
    playerRole = player;
    qDebug() << "Player Role : " << playerRole << '\n';
}

void Judge::init()
{
    memset(board, 0, sizeof(board));
}
