#ifndef JUDGE_H
#define JUDGE_H

#include <QObject>
#include <QDebug>

#define PIC_WIDTH 1068
#define PIC_HEIGHT 768
#define CHESS_BOARD_SIZE 9

#define BG_COLOR 0xecf9ff
#define STRESS_COLOR "color: rgb(96, 150, 180)" //0x6096b4

#define CUS_WHITE 0xFEDEFF
#define CUS_BLACK 0xFFFFFF

class Judge : public QObject
{
    Q_OBJECT

public:
    Judge (QObject *parent = nullptr);
    ~Judge();
    void init();

    int AI_role; // -1->white 1->black
    int board[CHESS_BOARD_SIZE + 2][CHESS_BOARD_SIZE + 2];
    int clickedRow, clickedColumn;

public slots:
    void setAI_role(int player);
};

#endif // JUDGE_H
