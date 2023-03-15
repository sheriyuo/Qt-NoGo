#include "judge.h"

Judge::Judge(QObject *parent) :
    QObject(parent)
{
    init();
}

Judge::~Judge()
{
    // delete this;
}

void Judge::init()
{
    for(int i = 0; i < (CHESSBOARD_SIZE + 2) * (CHESSBOARD_SIZE + 2); i++)
        blockLiberty[i].clear();
    for(int i = 0; i < (CHESSBOARD_SIZE + 2) * (CHESSBOARD_SIZE + 2); i++)
        chessBlock[i].clear();
    memset(board, 0, sizeof(board));
    memset(blockVis, 0, sizeof(blockVis));
    memset(chessBelong, -1, sizeof(chessBelong));
    blockCnt = 0;
}

int Judge::GridPoint(int x, int y) {return board[x][y];}

void Judge::PlaceAPiece(int x, int y)
{
    board[x][y] = CurColor();
    UpdateCurStep(x, y);
    curPlayer ^= 1;
}

bool Judge::IsEmpty(int x, int y)
{
    return !(bool)board[x][y];
}

bool Judge::IsInBoard(int x, int y)
{
    return x >= 0 && x < CHESSBOARD_SIZE && y >= 0 && y < CHESSBOARD_SIZE;
}

int Judge::CurColor() // 当前落棋颜色
{
    return curPlayer == 1 ? playerRole : -playerRole;
}

void Judge::CleanVis() // 当前落棋颜色
{
    for(int x : mergedBlock)
        blockVis[x] = 0;
    mergedBlock.clear();
}

void Judge::MergeSet(LibertySet &x, LibertySet y)
{
    if(x.size() < y.size()) std::swap(x, y);
    for(Item u : y) x.insert(u);
}

bool Judge::CheckVaild(int x, int y)
{
    Q_ASSERT(blockCnt <= CHESSBOARD_SIZE * CHESSBOARD_SIZE); // 检测 blockCnt
    if(!IsEmpty(x, y)) return false;

    int testNum = blockCnt + 1;
    blockLiberty[testNum].clear();

    for(int i = 0; i < 4; i++)
    {
        int xx = x + dx[i], yy = y + dy[i]; // 枚举相邻点
        if(!IsInBoard(xx, yy)) continue; // 判断边界条件

        int num = chessBelong[xx][yy];
        if(board[xx][yy] == 0) blockLiberty[testNum].insert(std::make_pair(xx, yy));
        else
        {
            if(board[xx][yy] == CurColor()) // 统计合并之后的气数
            {
                if(!blockVis[num])
                {
                    blockVis[num] = 1;
                    mergedBlock.push_back(num);
                    MergeSet(blockLiberty[testNum], blockLiberty[num]);
                }
            }
            else if(blockLiberty[num].size() == 1) // 直接吃掉的情况
                return false;
        }
    }

    if(mergedBlock.size())
        CleanVis();
    if(blockLiberty[testNum].find(std::make_pair(x, y)) != blockLiberty[testNum].end())
        blockLiberty[testNum].erase(std::make_pair(x, y));

    if(blockLiberty[testNum].size() == 0) // 没有气
        return false;
    blockLiberty[testNum].clear();
    return true;
}

void Judge::UpdateCurStep(int x, int y)
{
    board[x][y] = CurColor();
    blockCnt ++;
    chessBelong[x][y] = blockCnt;
    // qDebug() << x << "," << y << "->" << blockCnt;

    blockLiberty[blockCnt].clear();
    chessBlock[blockCnt].push_back(Point(x, y));
    // 新建棋子块

    for(int i = 0; i < 4; i++)
    {
        int xx = x + dx[i], yy = y + dy[i]; // 枚举相邻点
        if(!IsInBoard(xx, yy)) // 判断边界条件
            continue;

        int num = chessBelong[xx][yy];
        if(board[xx][yy] == 0)
        {
            blockLiberty[chessBelong[x][y]].insert(Point(xx, yy));
            continue;
        }
        if(board[xx][yy] == board[x][y]) // 相同颜色
        {
            if(chessBelong[x][y] != num) // 不同的块
                MergeBlock(chessBelong[x][y], num);
        }
        else
        {
            if(!blockVis[num]) // 没有更新的
            {
                blockVis[num] = 1;
                mergedBlock.push_back(num);
                blockLiberty[num].erase(Point(x, y));
            }
        }
    }

    if(mergedBlock.size())
        CleanVis();
    if(blockLiberty[chessBelong[x][y]].find(Point(x, y)) != blockLiberty[chessBelong[x][y]].end())
        blockLiberty[chessBelong[x][y]].erase(Point(x, y));

    for(int i = 0; i < CHESSBOARD_SIZE; i++)
        for(int j = 0; j < CHESSBOARD_SIZE; j++)
            if(IsEmpty(i, j)&&chessBelong[i][j] != -1)
            {
                // qDebug() << i << "," << j;
                Q_ASSERT(chessBelong[i][j] == -1);
            }

}

void Judge::MergeBlock(int x, int y) // 启发式合并
{
    Q_ASSERT(x >= 0 && y >= 0);
    Q_ASSERT(x != y);
    if(chessBlock[x].size() < chessBlock[y].size()) std::swap(x, y);
    MergeSet(blockLiberty[x], blockLiberty[y]);

    for(Item u : chessBlock[y])
    {
        chessBlock[x].push_back(u);
        chessBelong[u.first][u.second] = x;
    }
    // 合并

    chessBlock[y].clear();
    // 清空
}

void Judge::setPlayerRole(int player, int mode, int size)
{
    playerRole = player;
    if(player == -1) curPlayer = 0;
    else curPlayer = 1;
    // qDebug() << curPlayer;
    runMode = mode;
    CHESSBOARD_SIZE = size;
}
