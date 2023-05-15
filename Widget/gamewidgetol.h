#ifndef GAMEWIDGETOL_H
#define GAMEWIDGETOL_H

#include "widget/gamewidget.h"

namespace Ui { class GameWidgetOL; }

/*
*   @file: gamewidgetol.h
*   @brief: 声明 GameWidgetOL 类，
*           维护联机对局过程中的棋盘状态
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*/

class GameWidgetOL : public GameWidget
{
    Q_OBJECT

public:
    GameWidgetOL(Judge *j, Bot *b, QWidget *parent = nullptr);
    ~GameWidgetOL();

signals:

public slots:
    void on_resignButton_clicked() override; // 链接 resign 按钮的行为
    void on_restartButton_clicked() override; // 链接 restart 按钮的行为
    void on_sendButton_clicked(); // 链接 send 按钮的行为

    void playerTimeout() override; // 玩家超时的槽函数，用于链接计时器
    void remoteResign();  // 联机棋手投降

protected:
    /*
     * 弹出消息窗口
     * type=0 : player win
     * type=1 : player lose
     * type=5 : player resign
     * type=6 : remote player resgin
     * type=7 : cannot restart
    */
    void takeTurn(int row, int col) override; // 轮流下棋
    void sendMessage(int type) override;


private:
    Ui::GameWidgetOL *ui;
};

#endif // GAMEWIDGETOL_H
