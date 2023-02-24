#ifndef JUDGE_H
#define JUDGE_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <set>

#define PLAYER_TIMEOUT 10 // s
#define BOT_TIMEOUT 3      // s

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define CHESSBOARD_LEN 648
#define CHESSBOARD_SIZE 9

#define BG_COLOR 0xecf9ff
#define GRID_COLOR 0xbad7e9
#define GRID_THICKNESS (CHESSBOARD_LEN / CHESSBOARD_SIZE / 6)
#define ACCENT_COLOR "color: rgb(96, 150, 180)" //0x6096b4

#define CUS_BLACK 0xEAACB8
#define CUS_WHITE 0x7ACBF5

#define BTOL (double)0.8
    //(外边框与小正方形变长的比例 )
#define SQUARE_LEN ((double)(CHESSBOARD_LEN) / (CHESSBOARD_SIZE - 1 + 2 * BTOL ))
#define LEFT_UP ((double)(WINDOW_HEIGHT-CHESSBOARD_LEN ) / 2 + BTOL * SQUARE_LEN) //+SQUARE_LEN)

typedef std::pair<int, int> Item;

const int dx[4] = {1, 0, -1, 0};
const int dy[4] = {0, 1, 0, -1};

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

    int playerRole; // -1->white 1->black
    int curPlayer; // 0->bot 1->player -1->game over

public slots:
    void setPlayerRole(int player);

private:
    void CleanVis(); // 清空 mergedBlock
    void WriteCurStep(int x, int y); // 记录当前操作
    void MergeBlock(int x, int y); // 启发式合并
    void MergeSet(std::set<std::pair<int, int> > &x, std::set<std::pair<int, int> > y); // 启发式合并

    int board[CHESSBOARD_SIZE + 2][CHESSBOARD_SIZE + 2]; // 当前棋盘状态
    int chessBelong[CHESSBOARD_SIZE + 2][CHESSBOARD_SIZE + 2]; // 棋子属于的棋子块
    int blockVis[(CHESSBOARD_SIZE + 2) * (CHESSBOARD_SIZE + 2)]; // 棋子块至多只能累加一次气数
    int blockCnt; // 棋子块个数

    std::set<std::pair<int, int> >blockLiberty[(CHESSBOARD_SIZE + 2) * (CHESSBOARD_SIZE + 2)]; // checkValid() 计算好的气数
    std::vector<std::pair<int, int> >chessBlock[(CHESSBOARD_SIZE + 2) * (CHESSBOARD_SIZE + 2)]; // 棋子块的编号
    std::vector<int>mergedBlock;
};

#endif // JUDGE_H
