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
    void restartSingal(int index);
public slots:
    void on_restartButton_clicked();
    void startTimer();
    void playerTimeout();
    void botTimeout();

private:
    Ui::GameWidget *ui;
    void paintEvent(QPaintEvent *event) override;
    void drawChessboard(QPainter &painter);
    void drawBlack(QPainter &painter, double px, double py);
    void drawWhite(QPainter &painer, double px, double py);
    void drawDemo(QPainter &painter);
    // 监听鼠标坐标
    void mousePressEvent(QMouseEvent *event) override;
    void gameLose(int type = 0);
    void gameWin(int type = 0);

    /*
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
