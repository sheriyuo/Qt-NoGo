#ifndef JUDGE_H
#define JUDGE_H

#include <QObject>
#include <QDebug>

#define PIC_WIDTH 1068
#define PIC_HEIGHT 768
#define CHESS_BOARD_SIZE 9

#define BG_COLOR 0xecf9ff
#define STRESS_COLOR "color: rgb(96, 150, 180)" //0x6096b4

#define CUS_BLACK 0xEAACB8
#define CUS_WHITE 0x7ACBF5

const int dx[4] = {1, 0, -1, 0};
const int dy[4] = {0, 1, 0, -1};

class Judge : public QObject
{
    Q_OBJECT

public:
    Judge (QObject *parent = nullptr);
    ~Judge();
    void init();
    bool IsEmpty(int x, int y);
    int CurColor();
    bool CheckVaild(int x, int y); // 判断(x,y)是否可以下棋
    void UpdateCurStep(int x, int y); // 更新当前操作

    int playerRole; // -1->white 1->black
    int curPlayer; // 0->bot 1->player

public slots:
    void setPlayerRole(int player);

private:
    void WriteCurStep(int x, int y); // 记录当前操作
    void MergeBlock(int x, int y); // 启发式合并

    int board[CHESS_BOARD_SIZE + 2][CHESS_BOARD_SIZE + 2]; // 当前棋盘状态
    int chessBelong[CHESS_BOARD_SIZE + 2][CHESS_BOARD_SIZE + 2]; // 棋子属于的棋子块
    int blockLiberty[(CHESS_BOARD_SIZE + 2) * (CHESS_BOARD_SIZE + 2)]; // 棋子块的气数
    int blockCnt; // 棋子块个数
    std::vector<std::pair<int, int> >chessBlock[(CHESS_BOARD_SIZE + 2) * (CHESS_BOARD_SIZE + 2)]; // 棋子块的编号
};

#endif // JUDGE_H
