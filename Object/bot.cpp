#include "Object/bot.h"

/*
*   @file: bot.cpp
*   @brief: 声明 Bot 类，
*           控制下棋过程中的 AI 逻辑
*   @author: sheriyuo, ce-amtic
*   @time: 2023/5/16
*/

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

void Bot::dfsBoard(int x, int y)
{
    dfsVis[x][y] = 1;
    for(int i = 0; i < 4; i++)
    {
        int xx = x + dx[i], yy = y + dy[i]; // 枚举相邻点
        if(!judge->IsInBoard(xx, yy)) continue; // 判断边界条件
        if(!curBoard[xx][yy]) isEmpty[xx][yy] = 1;
        else if(curBoard[xx][yy] == curBoard[x][y] && !dfsVis[xx][yy]) dfsBoard(xx, yy);
    }
}

bool Bot::checkBoard(int x, int y)
{
    if(curBoard[x][y]) return false;
    int Allspace = 0;
    memset(dfsVis, 0, sizeof(dfsVis));
    for(int i = 0; i < 4; i++)
    {
        int xx = x + dx[i], yy = y + dy[i]; // 枚举相邻点
        if(!judge->IsInBoard(xx, yy)) continue; // 判断边界条件
        if(!curBoard[xx][yy])
        {
            Allspace ++;
            continue;
        }
        else if(!dfsVis[xx][yy])// 同色和异色的连通块
        {
            memset(isEmpty, 0, sizeof(isEmpty));
            dfsVis[x][y] = 1;
            isEmpty[x][y] = 1;
            dfsBoard(xx, yy);
            int space = 0;
            for(int x = 0; x < judge->CHESSBOARD_SIZE; x++)
                for(int y = 0; y < judge->CHESSBOARD_SIZE; y++)
                    space += isEmpty[x][y];
            if(space == 1) return 0;
            if(curBoard[xx][yy] == judge->CurColor()) Allspace += space - 1;
        }
    }
 //   qDebug() << x << "," << y << "->" <<judge->curPlayer << " " << judge->CurColor() <<" " << Allspace <<  "\n";
    return Allspace > 0; // 最后落子得有气
}

double Bot::judgeBoard() // 估价函数判断当前局面
{
    int botValid = 0, rivalValid = 0, bothValid = 0;

    for(int x = 0; x < judge->CHESSBOARD_SIZE; x++)
        for(int y = 0; y < judge->CHESSBOARD_SIZE; y++)
        {
            judge->curPlayer = 0;
            bool botCan = checkBoard(x, y);
            judge->curPlayer = 1;
            bool rivalCan = checkBoard(x, y);

            if(botCan ^ rivalCan) // 只有一人能落子
            {
                if(botCan) botValid ++;
                else rivalValid ++;
            }
            if(botCan && rivalCan)
                bothValid ++;
        }
    // 计算 bot 能走的格数

    if(botValid >= bothValid + rivalValid) return 1; // 必胜情况
    if(rivalValid >= bothValid + botValid) return 0; // 必败情况
    if(!botValid && !rivalValid) return 0.5; // 无法判断的情况

    //qDebug() << "bot: " << botValid << "human: " << rivalValid << "value: " << exp(-1.0 * botValid / (botValid + rivalValid));
    // 设 botvalid = x, rivalValid = y, 估价为 1-e^(-x/(x+y))
    return 1-exp(-1.0 * botValid / (botValid + rivalValid));
}

// minimax 搜索
double Bot::alphaBeta(int x, int y, double a, double b, int depth)
{
    time_t curTime = clock();
   // qDebug() << x <<" " <<y <<" "<<a<<" "<<b<<" "<<depth<<" "<<curTime<<" "<<searchStartTime;
    if((curTime - searchStartTime) / (BOT_TIMEOUT * 1000) > 0.9) return depth & 1 ? b : a; // 判断超时
    judge->curPlayer = depth & 1;
    if(depth == 3) return judgeBoard();
    if(depth & 1)
    {
        int op = 0;
        for(int xx = 0; xx < judge->CHESSBOARD_SIZE; xx++)
            for(int yy = 0; yy < judge->CHESSBOARD_SIZE; yy++)
                if(checkBoard(xx, yy))
                {
                    op = 1;
                    curBoard[xx][yy] = judge->CurColor();
                    b = std::min(b, alphaBeta(xx, yy, a, b, depth + 1));
                    judge->curPlayer = depth & 1;
                    curBoard[xx][yy] = 0;
                    if(a >= b) break;
                }
                else qDebug() << xx <<","<<yy<<"!?!?! false";
        if(!op) return judgeBoard();
        return a;
    }
    else
    {
        int op = 0;
        for(int xx = 0; xx < judge->CHESSBOARD_SIZE; xx++)
            for(int yy = 0; yy < judge->CHESSBOARD_SIZE; yy++)
                if(checkBoard(xx, yy))
                {
                    op = 1;
                    curBoard[xx][yy] = judge->CurColor();
                    double preA = a;
                    a = std::max(a, alphaBeta(xx, yy, a, b, depth + 1));
                    judge->curPlayer = depth & 1;
                    curBoard[xx][yy] = 0;
                    if(depth == 0 && a > preA)
                    {
                        finalx = xx, finaly = yy;
                       // qDebug() << a << " " << preA << " " << xx << " " << yy << " " << depth << " " << judge->curPlayer << " " << judge->CurColor() << " " << checkBoard(xx, yy) << "!!!!";
                        //for(int i=0;i<9;i++) qDebug()<<curBoard[i][0]<<curBoard[i][1]<<curBoard[i][2]<<curBoard[i][3]<<curBoard[i][4]<<curBoard[i][5];

                    }
                    //else qDebug() << a << " " << preA << " " << xx <<"," << yy <<"?";
                    if(a >= b) break;
                }
        if(!op) return judgeBoard();
        return b;
    }
}

void Bot::makeAMove()
{
    searchStartTime = clock(); // 计时器
    srand(time(0));

    for(int x = 0; x < judge->CHESSBOARD_SIZE; x++)
        for(int y = 0; y < judge->CHESSBOARD_SIZE; y++)
            curBoard[x][y] = judge->GridPoint(x, y);
    alphaBeta(0, 0, 0, 1, 0);
    judge->curPlayer = 0;
    qDebug() << finalx << "::" << finaly << " " << checkBoard(finalx, finaly) << "\n";
    checkBoard(finalx, finaly);
    if(!judge->CheckVaild(finalx, finaly))
    {
        emit timeout();
        return;
    }
    judge->PlaceAPiece(finalx, finaly);
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
        x = rand() % judge->CHESSBOARD_SIZE;
        y = rand() % judge->CHESSBOARD_SIZE;
    }
    while(!judge->CheckVaild(x, y));
    judge->PlaceAPiece(x, y);
}
