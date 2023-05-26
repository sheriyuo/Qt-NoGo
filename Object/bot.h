#ifndef BOT_H
#define BOT_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include "Object/judge.h"

/*
*   @file: bot.h
*   @brief: 声明 Bot 类，
*           控制下棋过程中的 AI 逻辑
*   @author: sheriyuo, ce-amtic
*   @time: 2023/5/25
*/

class Bot : public QThread
{
    Q_OBJECT

public:
    Bot (Judge *j, QObject *parent = nullptr);
    ~Bot();

    void run(); // run 函数是 bot 作为独立线程的入口, 通过 bot->start() 来启动这个独立线程, run 函数本身相当于 makeAMove

signals:
    void timeout();

public slots:

private:
    double eps = 0.03, alpha = 0.37, beta = 0.63;
    const double delta = 0.9;
    time_t searchStartTime;
    bool alphaBeta(); // Alpha-Beta 剪枝

    void makeRandomMove();
    double judgeBoard();
    double alphaBeta(double a, double b, int depth); // MinMax 搜索
    void readFromJudge();

    const int dx[8] = {0, 1, 0, -1}, dy[8] = {1, 0 ,-1, 0};
    bool checkBoard(int x, int y); // 可撤销的判断
    void dfsBoard(int x, int y);
    int curBoard[29][29];
    unsigned int dfsVis[29][29];
    bool isEmpty[29][29];

    int CurColor(); // 当前落棋颜色
    bool IsInBoard(int x, int y);
    int playerRole; // -1->white 1->black
    int curPlayer, curPlayerBak; // 0->bot 1->player -1->game over
    int CHESSBOARD_SIZE;
    int pointChecked;
    unsigned int dfsBoardTime = 0;

    int finalx, finaly;
    double finalv; // 最终决定的落子
    std::shared_ptr<Judge> judge;

    int maxDep;
    ItemVector chooseVec, pointVec[5];
};

#endif // BOT_H
