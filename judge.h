#ifndef JUDGE_H
#define JUDGE_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <set>
#include "network/networkserver.h"
#include "network/networksocket.h"

#define PLAYER_TIMEOUT 10  // s
#define BOT_TIMEOUT 1      // s

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define CHESSBOARD_LEN 648
// #define CHESSBOARD_SIZE 9

#define BG_COLOR 0xecf9ff
#define GRID_COLOR 0xbad7e9
// #define GRID_THICKNESS (CHESSBOARD_LEN / CHESSBOARD_SIZE / 6)
#define ACCENT_COLOR "color: rgb(96, 150, 180)" //0x6096b4

#define CUS_BLACK 0xEAACB8
#define CUS_WHITE 0x7ACBF5

#define BTOL (double)0.8
    //(外边框与小正方形变长的比例

static const int dx[4] = {1, 0, -1, 0};
static const int dy[4] = {0, 1, 0, -1};

typedef std::pair<int, int> Item;
typedef std::vector<Item> ItemVector;
typedef std::set<Item> LibertySet;
#define Point std::make_pair

class Judge : public QObject
{
    Q_OBJECT

public:
    Judge (QObject *parent = nullptr);
    ~Judge();
    void init();
    void UpdateCurStep(int x, int y); // 更新当前操作
    bool IsEmpty(int x, int y);
    bool IsInBoard(int x, int y);
    bool CheckVaild(int x, int y); // 判断(x,y)是否可以下棋
    int CurColor(); // 当前落棋颜色
    int GridPoint(int x, int y); // 访问 board 数组， 返回 (x, y) 的状态 ：0/-1/1
    void PlaceAPiece(int x, int y); // 编辑 board 数组，在 (x, y) 下 CurColor() 颜色的棋
    void SaveStep(int x,int y); // 记录当前在 (x, y) 下棋

    ItemVector getStep(); // 读取记录步数
    void updateStep(ItemVector newStep); // 更新记录步数并更改棋局

    int playerRole; // -1->white 1->black
    int curPlayer, curPlayerBak; // 0->bot 1->player -1->game over
    int CHESSBOARD_SIZE;
    int runMode;  // 0->PVE  1->PVP  2->Server(PVPOL)  3->Client(PVPOL)
    QString IP;
    quint16 PORT;
    NetworkServer *server;
    NetworkSocket *socket;
    QTcpSocket *lastClient = nullptr;

    int GRID_THICKNESS() {return (CHESSBOARD_LEN / CHESSBOARD_SIZE / 6);}
    double SQUARE_LEN() {return ((double)(CHESSBOARD_LEN) / (CHESSBOARD_SIZE - 1 + 2 * BTOL));}
    double LEFT_UP() {return ((double)(WINDOW_HEIGHT-CHESSBOARD_LEN ) / 2 + BTOL * SQUARE_LEN());}
    double RIGHT_UP() {return ((double)(WINDOW_HEIGHT-CHESSBOARD_LEN ) / 2 + CHESSBOARD_LEN);}

signals:
    void modifiedCB(); // 棋盘更新信号

    // 联机通信协议
    void READY_OP(int inviterRole);
    void READY_OP_ForInviter();
    void REJECT_OP();
    void MOVE_OP();

public slots:
    void setPlayerRole(int player);
    void recDataFromClient(QTcpSocket* client, NetworkData data);
    void recData(NetworkData data);

private:
    void CleanVis(); // 清空 mergedBlock
    void WriteCurStep(int x, int y); // 记录当前操作
    void MergeBlock(int x, int y); // 启发式合并
    void MergeSet(LibertySet &x, LibertySet y); // 启发式合并

    int board[52][52]; // 当前棋盘状态
    int chessBelong[52][52]; // 棋子属于的棋子块
    int blockVis[(52) * (52)]; // 棋子块至多只能累加一次气数
    int blockCnt; // 棋子块个数

    LibertySet blockLiberty[(52) * (52)]; // 气的 Set
    ItemVector chessBlock[(52) * (52)]; // 棋子块的编号
    ItemVector savedStep; // 记录棋局的 vector<pair<int,int> >
    std::vector<int>mergedBlock;
};

#endif // JUDGE_H
