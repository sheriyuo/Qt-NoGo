#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "messagebox.h"
#include "judge.h"
#include <math.h>

// 玩家的计时器
static QTimer *timerForPlayer;
// 将 bot 的计时器内置于 class Bot 中
// static QTimer *timerForBot;

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // 设置 restart button 的样式，无边框在 ui 文件中设置
    ui->restartButton->setStyleSheet(ACCENT_COLOR); // 文字颜色
    int H = (double)WINDOW_HEIGHT / 31 * 2,
        W = (double)WINDOW_HEIGHT / 31 * 6;
    int X = LEFT_UP + CHESSBOARD_LEN + (double)(WINDOW_WIDTH - WINDOW_HEIGHT) / 2 - (double)WINDOW_HEIGHT / 31 * 3.5,
        Y = (double)WINDOW_HEIGHT / 20 * 8 - (double)WINDOW_HEIGHT / 31;
    ui->restartButton->setGeometry(QRect(QPoint(X, Y), QSize(W, H))); // 位置

    judge = new Judge;
    bot = new Bot;
    bot->judge = judge;

    // 链接计时器
    timerForPlayer = new QTimer;
    connect(timerForPlayer,&QTimer::timeout, this, &GameWidget::playerTimeout);
    connect(bot, &Bot::timeout, this, &GameWidget::botTimeout);
}

GameWidget::~GameWidget()
{
    delete ui;
}

// 监听鼠标点击实现落子
void GameWidget::mousePressEvent(QMouseEvent *event)
{
    if(judge->curPlayer == -1) return; // 判断游戏结束
    double x = event->position().x(), y = event->position().y();
    int row = 0,column = 0;
    double checklen = SQUARE_LEN / 2;
    for(int i = 1 ; i <=  CHESSBOARD_SIZE ; i++)
    {
        int xi = LEFT_UP + (i-1) * SQUARE_LEN;
        if(abs ( x - xi ) < checklen)
        {
            row = i;
            break;
        }
    }
    for(int i = 1 ; i <= CHESSBOARD_SIZE ; i++)
    {
        int yi = LEFT_UP + ( i - 1 ) * SQUARE_LEN;
        if(abs( y - yi ) < checklen)
        {
            column = i;
            break;
        }
    }

    // 此处实现轮流下棋
    if(row - 1 < 0 || column - 1 < 0 || row - 1 >= CHESSBOARD_SIZE || column - 1 >= CHESSBOARD_SIZE)
        return;
    /*if(judge->IsEmpty(row-1, column-1)){
        timerForPlayer->stop();

        bool judgement = judge->CheckVaild(row-1, column-1);
        judge->PlaceAPiece(row-1, column-1);
        update();
        if(!judgement) // suicide
        {
            gameLose();
            return;
        }

        bot->makeRandomMove();
        timerForPlayer->start(PLAYER_TIMEOUT * 1000);

        update();
    }*/
    if(judge->CheckVaild(row-1, column-1)){
        timerForPlayer->stop();
        judge->PlaceAPiece(row-1, column-1);
        update();

        bot->makeRandomMove();
        update();
        timerForPlayer->start(PLAYER_TIMEOUT * 1000);
    }
    else
    {
        sendMessage(2);
    }
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    // 绘制棋盘
    drawChessboard(painter);

    // drawDemo(painter);
    // 绘制棋子
    for(int i = 0; i < CHESSBOARD_SIZE; i++)
        for(int j = 0; j < CHESSBOARD_SIZE; j++)
        {
            if(this->judge->GridPoint(i, j) == -1)
            {
                drawWhite(painter, i, j);
            }
            if(this->judge->GridPoint(i, j) == 1)
            {
                drawBlack(painter, i, j);
            }

        }
}

void GameWidget :: drawChessboard(QPainter &painter)
{

    int number=CHESSBOARD_SIZE;
    double left_up,L;
    left_up=(WINDOW_HEIGHT-CHESSBOARD_LEN)/2;
    L=SQUARE_LEN;

    //背景
    painter.setBrush(QColor(BG_COLOR));
    painter.drawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    painter.setPen(QPen(QColor(GRID_COLOR), GRID_THICKNESS, Qt::SolidLine));
    painter.drawRect(left_up, left_up, CHESSBOARD_LEN ,CHESSBOARD_LEN);

    // 水平线
    for(int i = 0 ; i < number ; i++)
    {

        painter.drawLine( LEFT_UP , LEFT_UP + i * L , LEFT_UP + ( number - 1 ) * L , LEFT_UP + i * L );
    }

    //垂直线
    for(int i = 0 ; i < number ; i++)
    {

        painter.drawLine( LEFT_UP + i * L , LEFT_UP ,  LEFT_UP + i * L ,  LEFT_UP + ( number - 1 ) * L );
    }
}

void GameWidget::drawWhite(QPainter &painter, double i, double j)
{

    double Size = SQUARE_LEN * 0.7, Strength = Size / 4;

    double xi = LEFT_UP + ( i - 0.35 ) * SQUARE_LEN;
    double yi = LEFT_UP + ( j - 0.35 ) * SQUARE_LEN;

    painter.setPen(QPen(QColor(CUS_WHITE), Strength, Qt::SolidLine));
    painter.setBrush(QColor(BG_COLOR));

    painter.drawEllipse(xi, yi, Size, Size);
}

void GameWidget::drawBlack(QPainter &painter, double i, double j)
{

    double Size = SQUARE_LEN * 0.7, Strength = Size / 4;
    double xi = LEFT_UP + ( i - 0.35 ) * SQUARE_LEN;
    double yi = LEFT_UP + ( j - 0.35 ) * SQUARE_LEN;

    painter.setPen(QPen(QColor(CUS_BLACK), Strength, Qt::SolidLine));
    painter.setBrush(QColor(BG_COLOR));

    painter.drawEllipse(xi, yi, Size, Size);
}

void GameWidget::drawDemo(QPainter &painter)
{
    for(int i = 0; i < CHESSBOARD_SIZE; i++)
        for(int j = 0; j < CHESSBOARD_SIZE; j++)
        {
            if(judge->GridPoint(i, j)) return;
        }
    srand(time(0));
    for(int i = 0; i < CHESSBOARD_SIZE; i++)
        for(int j = 0; j < CHESSBOARD_SIZE; j++)
        {
            if(rand() % 3) continue;
            if((i + j) & 1)
            {
                drawWhite(painter, i, j);
            }
            else
            {
                drawBlack(painter, i, j);
            }

        }
}

void GameWidget::playerTimeout() {if(judge->curPlayer >= 0) gameLose(1);}
void GameWidget::gameLose(int type)
{
    if(!type) sendMessage(1);
    else sendMessage(3);
    judge->curPlayer = -1;
}

void GameWidget::botTimeout() {if(judge->curPlayer >= 0) gameWin(1);}
void GameWidget::gameWin(int type)
{
    if(!type) sendMessage(0);
    else sendMessage(4);
    judge->curPlayer = -1;
}

/*
 * 弹出消息窗口
 * type=0 : player win
 * type=1 : player lose
 * type=2 : invalid position
 * type=3 : player timeout
 * type=4 : bot timeout
*/
void GameWidget::sendMessage(int type)
{
    // mess = new MessageBox(QString("qwq"), this);
    switch(type)
    {
        case 0:
            mess = new MessageBox(QString("Congratulations!\n\nYou WIN"), 3000, this);
            break;
        case 1:
            mess = new MessageBox(QString("Sorry!\n\nYou LOSE"), 3000, this);
            break;
        case 2:
            mess = new MessageBox(QString("You cannot place a \npiece there!"), 2000, this);
            break;
        case 3:
            mess = new MessageBox(QString("TIME'S UP!\n\nYou LOSE"), 3000, this);
            break;
        case 4:
            mess = new MessageBox(QString("Bot failed to make\na move.\nYou WIN"), 3000, this);
            break;
    }
    mess->show();
}

void GameWidget::on_restartButton_clicked()
{
    emit restartSingal(0);
    judge->init();
    this->close();
}

void GameWidget::startTimer()
{
    timerForPlayer->start(PLAYER_TIMEOUT * 1000);
}
