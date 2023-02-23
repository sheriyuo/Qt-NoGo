#ifndef JUDGE_H
#define JUDGE_H

#include <QObject>
#include <QDebug>

#define PIC_WIDTH 1024
#define PIC_HEIGHT 768
#define CHESSBOARD_LEN 648
#define CHESSBOARD_SIZE 9
#define CHESSBOARD_BOARDER (CHESSBOARD_LEN / CHESSBOARD_SIZE * (CHESSBOARD_SIZE + 1))
#define CHESSBOARD_MARGIN

#define BG_COLOR 0xecf9ff
#define GRID_COLOR 0xbad7e9
#define GRID_THICKNESS (CHESSBOARD_LEN / CHESSBOARD_SIZE / 10)
#define ACCENT_COLOR "color: rgb(96, 150, 180)" //0x6096b4

#define CUS_BLACK 0xEAACB8
#define CUS_WHITE 0x7ACBF5

class Judge : public QObject
{
    Q_OBJECT

public:
    Judge (QObject *parent = nullptr);
    ~Judge();
    void init();

    int playerRole; // -1->white 1->black
    int board[CHESSBOARD_SIZE + 2][CHESSBOARD_SIZE + 2];
    int clickedRow, clickedColumn;

public slots:
    void setPlayerRole(int player);
};

#endif // JUDGE_H
