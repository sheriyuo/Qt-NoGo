#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QPushButton>
#include <QPainter>
#include <QWidget>
#include <QMouseEvent>
#include "judge.h"
#include "bot.h"
#include "messagebox.h"

namespace Ui { class GameWidget; }

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

    Judge *judge;
    Bot *bot;

signals:
    void restartSingal(int index);   // 用于发送信号，使得 judge 得以重置
    void resignSingal(int index);   // 用于发送信号，使得 judge 得以重置

public slots:
    void on_restartButton_clicked(); // 链接 restart 按钮的行为
    void on_resignButton_clicked(); // 链接 resign 按钮的行为
    void startTimer();    // 当对局开始时，无论谁先手（机器下第一个棋子认为不需要时间），都打开玩家的计时器
    void playerTimeout(); // 玩家超时的槽函数，用于链接计时器
    void botTimeout();    // bot超时的槽函数，用于链接计时器

private:
    Ui::GameWidget *ui;
    void paintEvent(QPaintEvent *event) override; // 界面绘制
    void drawChessboard(QPainter &painter);
    void drawBlack(QPainter &painter, double px, double py);
    void drawWhite(QPainter &painer, double px, double py);
    void drawDemo(QPainter &painter);

    void mousePressEvent(QMouseEvent *event) override; // 监听鼠标坐标
    void gameLose(int type = 0); // 输掉游戏， type=1 表示超时
    void gameWin(int type = 0);  // 赢了游戏， type=1 表示机器超时

    /*
     * 发送消息
     * type=0 : player win
     * type=1 : player lose
     * type=2 : invalid position
     * type=3 : player timeout
     * type=4 : bot timeout
    */
    void sendMessage(int type);

    MessageBox *mess;
};

#endif // GAMEWIDGET_H
