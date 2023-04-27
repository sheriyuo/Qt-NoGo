#include "bot.h"

Bot::Bot(Judge *j, QObject *parent) :
    QObject(parent),
    judge(j)
{
    // none
}

Bot::~Bot()
{
    delete this;
}

void Bot::makeFirstMove(int player)
{
    if(player == -1) // bot 先手
    {
        if(judge->runMode == 0) makeAMove();
        if(judge->runMode == 1) judge->curPlayer ^= 1;
    }
}

void Bot::makeAMove() {makeRandomMove();}
void Bot::makeRandomMove()
{
    srand(time(0));
    time_t startTime = clock(); // 计时器
    int x, y;
    do
    {
        time_t curTime = clock();
        if(curTime - startTime > BOT_TIMEOUT * 1000)
        {
            emit timeout();
            return;
        }
        x = rand() % judge->CHESSBOARD_SIZE;
        y = rand() % judge->CHESSBOARD_SIZE;
    }
    while(!judge->CheckVaild(x, y));
    judge->PlaceAPiece(x, y);
}
