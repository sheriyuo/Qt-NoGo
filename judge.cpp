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

void Judge::setAI_role(int player)
{
    AI_role = -player;
    qDebug() << "AI_role : " << AI_role << '\n';
}

void Judge::init()
{
    memset(board, 0, sizeof(board));
}
