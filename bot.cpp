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
    int x, y;
    do{
        x = rand() % CHESSBOARD_SIZE;
        y = rand() % CHESSBOARD_SIZE;
    } while(!judge->CheckVaild(x, y));
    qDebug() << judge->CheckVaild(x, y) << '\n';
    judge->PlaceAPiece(x, y);
}
