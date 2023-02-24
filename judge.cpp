#include "judge.h"

Judge::Judge(QObject *parent) :
    QObject(parent)
{
    memset(board, 0, sizeof(board));
}

Judge::~Judge()
{
    delete this;
}

void Judge::init()
{
    memset(board, 0, sizeof(board));
    memset(blockLiberty, 0, sizeof(blockLiberty));
    memset(chessBelong, 0, sizeof(chessBelong));
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

bool Judge::CheckVaild(int x, int y)
{
    // qDebug() << "awa\n";
    if(!IsEmpty(x, y)) return false;
    // qDebug() << "qaq\n";
    int spaceCnt = 0;
    int libertySum = 0;

    for(int i = 0; i < 4; i++)
    {
        int xx = x + dx[i], yy = y + dy[i]; // 枚举相邻点
        if(!IsInBoard(xx, yy)) continue; // 判断边界条件

        if(board[xx][yy] == 0) spaceCnt ++;
        else
        {
            if(board[xx][yy] == CurColor()) // 统计合并之后的气数
                libertySum += blockLiberty[chessBelong[xx][yy]] - 1; // 去掉合并用去的气数
            else if(blockLiberty[chessBelong[xx][yy]] == 1) // 直接吃掉的情况
            {
                return false;
            }
           // qDebug() << xx << ' ' << yy << ' ' << blockLiberty[chessBelong[xx][yy]] << '\n';
        }
    }

    qDebug() << spaceCnt << ' ' << libertySum << '\n';
    if(spaceCnt + libertySum == 0) // 没有气
        return false;
    return true;
}

static bool vis[(CHESSBOARD_SIZE + 2) * (CHESSBOARD_SIZE + 2)];
void Judge::UpdateCurStep(int x, int y)
{
    memset(vis, 0, sizeof(vis));

    board[x][y] = CurColor();
    blockCnt ++;
    chessBelong[x][y] = blockCnt;
    blockLiberty[blockCnt] = 4;
    chessBlock[blockCnt].push_back(std::make_pair(x, y));
    // 新建棋子块

    for(int i = 0; i < 4; i++)
    {
        int xx = x + dx[i], yy = y + dy[i]; // 枚举相邻点
        if(!IsInBoard(xx, yy)) // 判断边界条件
        {
            blockLiberty[blockCnt] --;
            continue;
        }
        if(board[xx][yy] == 0) continue;

        if(board[xx][yy] == board[x][y])
        {
            if(!vis[chessBelong[xx][yy]]){
                vis[chessBelong[xx][yy]] = 1;
                MergeBlock(chessBelong[x][y], chessBelong[xx][yy]);
            }
        }
        else
        {
            blockLiberty[chessBelong[x][y]] --;
            if(!vis[chessBelong[xx][yy]])
            {
                vis[chessBelong[xx][yy]] = 1;
                blockLiberty[chessBelong[xx][yy]] --;
            }
        }
    }
}

void Judge::MergeBlock(int x, int y) // 启发式合并
{
    if(chessBlock[x].size() < chessBlock[y].size()) std::swap(x, y);
    blockLiberty[x] += blockLiberty[y] - 2; // 去掉合并的 2 个气

    typedef std::pair<int, int> Item;
    for(Item u : chessBlock[y])
    {
        chessBlock[x].push_back(u);
        chessBelong[u.first][u.second] = x;
    }
    // 合并

    blockLiberty[y] = 0;
    chessBlock[y].clear();
    // 清空
}

void Judge::setPlayerRole(int player)
{
    playerRole = player;
    if(player == -1) curPlayer = 0;
    else curPlayer = 1;
    // qDebug() << "Player Role : " << playerRole << '\n';
}
