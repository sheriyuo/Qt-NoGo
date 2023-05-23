#ifndef BOT_H
#define BOT_H

#include <QObject>
#include <QDebug>
#include "Object/judge.h"

/*
*   @file: bot.h
*   @brief: 声明 Bot 类，
*           控制下棋过程中的 AI 逻辑
*   @author: sheriyuo, ce-amtic
*   @time: 2023/5/16
*/

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
    double alpha = 0.3, beta = 0.7;
    time_t searchStartTime;
    bool alphaBeta(); // Alpha-Beta 剪枝

    void makeRandomMove();
    double judgeBoard();
    double alphaBeta(int x, int y, double a, double b, int depth); // MinMax 搜索

    const int dx[4] = {0, 1, 0, -1}, dy[4] = {1, 0 ,-1, 0};
    bool checkBoard(int x, int y); // 可撤销的判断
    void dfsBoard(int x, int y);
    int curBoard[29][29];
    bool isEmpty[29][29], dfsVis[29][29];

    int finalx, finaly; // 最终决定的落子
    std::shared_ptr<Judge> judge;
};

#endif // BOT_H
