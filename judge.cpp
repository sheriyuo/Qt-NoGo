#include "judge.h"

/*
*   @file: judge.cpp
*   @brief: 声明 Judge 类，
*           控制下棋过程中的逻辑，包括进程调度，局面判断
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*/

Judge::Judge(QObject *parent) :
    QObject(parent)
{
    CHESSBOARD_SIZE = 9;
    runMode = 0;
    // IP = "127.0.0.1";
    foreach (QHostAddress address, QHostInfo::fromName(QHostInfo::localHostName()).addresses()) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            IP = address.toString();
        }
    }
    PORT = 1919;
    server = new NetworkServer(this);
    socket = new NetworkSocket(new QTcpSocket(), this);
    srand(time(0) + clock());
    // usrnameOL = QString("OnlinePlayer") + QString::number(QRandomGenerator::global()->bounded(89999) + 10000);
    usrnameOL = QHostInfo::localHostName();

    connect(server, &NetworkServer::receive, this, &Judge::recDataFromClient);
    connect(socket, &NetworkSocket::receive, this, &Judge::recData);

    init();
}

Judge::~Judge()
{
    delete this;
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
    savedStep.clear();
    blockCnt = 0;
    curPlayer = 0;
}

int Judge::GridPoint(int x, int y) {return board[x][y];}

void Judge::SaveStep(int x,int y) // 存下当前步数
{
    savedStep.push_back(Point(x,y));
}

void Judge::PlaceAPiece(int x, int y)
{
    board[x][y] = CurColor();
    UpdateCurStep(x, y);
    SaveStep(x, y);
    curPlayer ^= 1;
    emit modifiedCB();
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
    //Q_ASSERT(blockCnt <= CHESSBOARD_SIZE * CHESSBOARD_SIZE); // 检测 blockCnt
    if(!IsEmpty(x, y)) return false;

    int testNum = blockCnt + 1;
    blockLiberty[testNum].clear();

    for(int i = 0; i < 4; i++)
    {
        int xx = x + dx[i], yy = y + dy[i]; // 枚举相邻点
        if(!IsInBoard(xx, yy)) continue; // 判断边界条件

        int num = chessBelong[xx][yy];
        if(board[xx][yy] == 0) blockLiberty[testNum].insert(Point(xx, yy));
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
    if(blockLiberty[testNum].find(Point(x, y)) != blockLiberty[testNum].end())
        blockLiberty[testNum].erase(Point(x, y));

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

void Judge::setPlayerRole(int player)
{
    playerRole = player;
    if(player == -1) curPlayer = 0;
    else curPlayer = 1;
}

// 读档相关
ItemVector Judge::getStep()
{
    return savedStep;
}
// 更新 savedStep 并更新 board 数组
void Judge::updateStep(int newPlayerRole, int newRunMode, ItemVector newStep, char newState)
{
    savedStep.clear();
    init(); // 更新 savedStep
    setPlayerRole(newPlayerRole);
    runMode = newRunMode;

    for(Item cur : newStep)
    {
        Judge::PlaceAPiece(cur.first, cur.second); // 重新绘制棋盘并处理状态
    }

    loadState = newState;
    // timerForPlayer 在 gamewidget 里面，要在外面 init。
    // loadState 也要在 gamewidget 里面判断是否终局。
}

// 网络库相关
void Judge::recDataFromClient(QTcpSocket* client, NetworkData data)
{
    lastClient = client;
    recData(data);
}
void Judge::recData(NetworkData d)
{
    int row, col;
    switch(d.op)
    {
    case OPCODE::READY_OP:
        oppoOL = d.data1;
        if(d.data2 != ""){
            emit READY_OP(d);
            if(curPlayer == -1) send(NetworkData(OPCODE::REJECT_OP, "", ""));
        }
        else emit READY_OP_ForInviter();
        break;
    case OPCODE::REJECT_OP:
        emit REJECT_OP();
        break;
    case OPCODE::MOVE_OP:
        row = QChar(d.data1[0]).unicode()-'A', col = QChar(d.data1[1]).unicode()-'1';
        PlaceAPiece(row, col);
        emit MOVE_OP();
        break;
    case OPCODE::CHAT_OP:
        if(d.data1 != "") emit CHAT_OP(d);
        break;
    case OPCODE::TIMEOUT_END_OP:
        if(d.data2 != "")
        {
            if(runMode == 2) {server->send(lastClient, NetworkData(OPCODE::TIMEOUT_END_OP, usrnameOL, ""));}
            else {socket->send(NetworkData(OPCODE::TIMEOUT_END_OP, usrnameOL, ""));}
            emit TIMEOUT_END_OP();
        }
        break;
    case OPCODE::GIVEUP_OP:
        if(runMode == 2) {server->send(lastClient, NetworkData(OPCODE::GIVEUP_END_OP, usrnameOL, "Sorry you resign"));}
        else {socket->send(NetworkData(OPCODE::GIVEUP_END_OP, usrnameOL, "Sorry you resign"));}
        emit GIVEUP_OP();
        break;
    case OPCODE::GIVEUP_END_OP:
        if(d.data2 != "")
        {
            if(runMode == 2) {server->send(lastClient, NetworkData(OPCODE::GIVEUP_END_OP, usrnameOL, ""));}
            else {socket->send(NetworkData(OPCODE::GIVEUP_END_OP, usrnameOL, ""));}
        }
        break;
    case OPCODE::LEAVE_OP:
        clearLink();
        emit LEAVE_OP();
        break;
//    case OPCODE::SUICIDE_END_OP:
    }
}
void Judge::send(NetworkData d)
{
    if(runMode != 2 && runMode != 3) return;
    if(runMode == 2) server->send(lastClient, d);
    else socket->send(d);
}
void Judge::clearLink()
{
    if(runMode == 2)
    {
        if(lastClient != nullptr) server->leave(lastClient);
        if(server->isListening()) server->close();
        lastClient = nullptr;
    }
    if(runMode == 3)
    {
        socket->bye();
        socketConnected = false;
    }
    oppoOL = "";
}
