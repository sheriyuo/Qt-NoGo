#include "judge.h"

Judge::Judge(QObject *parent) :
    QObject(parent)
{
    memset(board, 0, sizeof(board));
}

Judge::~Judge()
{
    for(int i = 1; i <= blockCnt; i++)
        chessBlock[i].clear();
}

void Judge::setPlayerRole(int player)
{
    playerRole = player;
    qDebug() << "Player Role : " << playerRole << '\n';
}

void Judge::init()
{
    memset(board, 0, sizeof(board));
    blockCnt = 0;
}

void Judge::UpdateCurStep(int x, int y)
{
    int allEmpty = 1;
    for(int i = 0; i < 4; i++)
    {
        int xx = x + dx[i], yy = y + dy[i]; // 枚举相邻点
        if(!IsEmpty(xx, yy)) allEmpty = 0;
    }
    board[x][y] = curPlayer ? playerRole : 2 - playerRole;
    blockCnt ++;
    chessBelong[x][y] = blockCnt;
    chessBlock[blockCnt].push_back(std::make_pair(x, y));
    // 新建棋子块

    if(allEmpty == 1)
        return;

    for(int i = 0; i < 4; i++)
    {
        int xx = x + dx[i], yy = y + dy[i]; // 枚举相邻点
        if(board[xx][yy] == board[x][y])
            MergeBlock(chessBelong[x][y], chessBelong[xx][yy]);
    }
}

void Judge::MergeBlock(int x, int y)
{
    if(chessBlock[x].size() < chessBlock[y].size()) std::swap(x, y);

}
