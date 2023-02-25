#include "bot.h"
#include "judge.h"

Bot::Bot(QObject *parent) :
    QObject(parent)
{
    /*empty*/
}

Bot::~Bot()
{
    delete this;
}

void Bot::makeFirstMove(int player){
    if(player == -1) // bot 先手
    {
        makeRandomMove();
    }
}

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
        x = rand() % CHESSBOARD_SIZE;
        y = rand() % CHESSBOARD_SIZE;
    }
    while(!judge->CheckVaild(x, y));
    judge->PlaceAPiece(x, y);
}
