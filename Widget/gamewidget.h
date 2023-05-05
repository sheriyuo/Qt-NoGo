#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QPushButton>
#include <QPainter>
#include <QWidget>
#include <QMouseEvent>
#include <math.h>
#include <ctime>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "Object/judge.h"
#include "Object/bot.h"
#include "DialogBox/messagebox.h"

/*
*   @file: gamewidget.h
*   @brief: 声明 GameWidget 类，
*           维护对局过程中的棋盘状态，控制信号槽及文件读写
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*/

namespace Ui { class GameWidget; }

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    GameWidget(Judge *j, Bot *b, QWidget *parent = nullptr);
    ~GameWidget();

signals:
    void restartSingal(int index);   // 用于发送信号，使得 judge 得以重置
    void resignSingal(int index);   // 用于发送信号，使得 judge 得以重置
    void mousePress();              // 鼠标点击信号，用于刷新界面
    void turnForBot();              // 有请 Bot 表演

public slots:
    void on_loadButton_clicked(); // 链接 load 按钮的行为
    void on_saveButton_clicked(); // 链接 save 按钮的行为
    void on_resignButton_clicked_OFFL(); // 链接 resign 按钮的行为 (offline)
    void on_restartButton_clicked_OFFL(); // 链接 restart 按钮的行为 (offline)
    void on_resignButton_clicked_OL();
    void on_restartButton_clicked_OL();
    void on_sendButton_clicked(); // 链接 send 按钮的行为

    void startTimer();    // 当对局开始时，无论谁先手（机器下第一个棋子认为不需要时间），都打开玩家的计时器
    void playerTimeout_OFFL(); // 玩家超时的槽函数，用于链接计时器 (offline)
    void playerTimeout_OL();
    void botTimeout();    // bot超时的槽函数，用于链接计时器
    void closeMB();       // 再次点击棋盘时关闭消息弹窗
    void updateCB();      // 更新棋盘
    void updateBar();     // 更新倒计时进度条

    // 联机相关
    void goOL();
    void goOFFL();
    void remoteResign();

private:
    Ui::GameWidget *ui;
    void paintEvent(QPaintEvent *event) override; // 界面绘制
    void drawChessboard(QPainter &painter);
    void drawBlack(QPainter &painter, double px, double py);
    void drawWhite(QPainter &painer, double px, double py);
    void drawheat(QPainter &painer, double px, double py);


    void drawDemo(QPainter &painter);
    void setColorForBar(); // 计时器的颜色与当前执棋颜色一致

    void mousePressEvent(QMouseEvent *event) override; // 监听鼠标坐标
    void gameLose(int type = 0); // 输掉游戏（0->PVE 1->PVP）
    void gameWin(int type = 0);  // 赢了游戏（0->PVE 1->PVP）

    void dataToString(); // 编码
    void stringToData(); // 解码

    /*
     * 弹出消息窗口
     * type=0 : player win (online)
     * type=1 : player lose (online)
     * type=2 : invalid position
     * type=3 : player timeout
     * type=4 : bot timeout
     * type=5 : player resign (online / offline)
     * type=6 : remote player resgin (online)
     * type=7 : cannot restart (online)
    */
    void sendMessage(int type);

    Judge *judge;
    Bot *bot;
    MessageBox *mess = nullptr;
    int columnX, columnY, buttonW, buttonH;
    QPixmap logoImg;

    // 存储一个临时的存档
    char strState;
    char dataStr[28 * 28 * 3 + 5];
    ItemVector dataVec;
};

#endif // GAMEWIDGET_H