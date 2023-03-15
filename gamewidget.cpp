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
    setWindowIcon(QIcon(":/img/icon.png"));

    // 设置 restart button 的样式，无边框在 ui 文件中设置
    ui->restartButton->setStyleSheet(ACCENT_COLOR); // 文字颜色

    int X1 = columnX - buttonW / 2,
        Y1 = columnY - buttonH / 2 + WINDOW_HEIGHT * 0.16;
    ui->restartButton->setGeometry(QRect(QPoint(X1, Y1), QSize(buttonW, buttonH))); // 位置

    //设置 resign button 的样式，无边框在 ui 文件中设置
    ui->resignButton->setStyleSheet(ACCENT_COLOR); // 文字颜色
    int X2 = X1,
        Y2 = Y1 + WINDOW_HEIGHT * 0.15;
    ui->resignButton->setGeometry(QRect(QPoint(X2, Y2), QSize(buttonW, buttonH))); // 位置

    judge = new Judge;
    bot = new Bot;
    bot->judge = judge;
    mess = nullptr;

    // 链接计时器
    timerForPlayer = new QTimer;
    connect(timerForPlayer,&QTimer::timeout, this, &GameWidget::playerTimeout);
    connect(bot, &Bot::timeout, this, &GameWidget::botTimeout);
    // connect(mess, &MessageBox::timeUpClose, this, &GameWidget::mousePress);
}

GameWidget::~GameWidget()
{
    delete ui;
}

// 监听鼠标点击实现落子
void GameWidget::mousePressEvent(QMouseEvent *event)
{
    emit mousePress();

    if(judge->curPlayer == -1) return; // 判断游戏结束
    double x = event->position().x(), y = event->position().y();
    int row = 0,column = 0;
    double checklen = SQUARE_LEN() / 2;
    for(int i = 1 ; i <=  CHESSBOARD_SIZE ; i++)
    {
        int xi = LEFT_UP() + (i-1) * SQUARE_LEN();
        if(abs ( x - xi ) < checklen)
        {
            row = i;
            break;
        }
    }
    for(int i = 1 ; i <= CHESSBOARD_SIZE ; i++)
    {
        int yi = LEFT_UP() + ( i - 1 ) * SQUARE_LEN();
        if(abs( y - yi ) < checklen)
        {
            column = i;
            break;
        }
    }

    // 此处实现轮流下棋
    if(row - 1 < 0 || column - 1 < 0 || row - 1 >= CHESSBOARD_SIZE || column - 1 >= CHESSBOARD_SIZE)
        return;

    if(judge->CheckVaild(row-1, column-1)){
        timerForPlayer->stop();
        judge->PlaceAPiece(row-1, column-1);

        if(!judge->runMode){
            bot->makeRandomMove();
        }
        timerForPlayer->start(PLAYER_TIMEOUT * 1000);
    }
    else
    {
        sendMessage(2);
    }

    update();
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    // 绘制 demo
        drawChessboard(painter);

    QPixmap logoImg;
    logoImg.load(":/img/logo.png");

    int logoBoardW = (WINDOW_WIDTH - RIGHT_UP()) * 0.8; // 等比缩放
    int logoBoardH = (double)logoImg.height() * logoBoardW / logoImg.width();
    // 右边 LOGO 大小

    // qDebug() << logoBoardW << " " << logoBoardH << "\n";

    int LOGO_X = columnX - logoBoardW / 2;
    int LOGO_Y = columnY - logoBoardH / 2;
    painter.drawPixmap(LOGO_X, LOGO_Y, logoBoardW, logoBoardH, logoImg);

    if(CHESSBOARD_SIZE == 28)
        drawDemo(painter);
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
    L=SQUARE_LEN();

    //背景
    painter.setBrush(QColor(BG_COLOR));
    painter.drawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    painter.setPen(QPen(QColor(GRID_COLOR), GRID_THICKNESS(), Qt::SolidLine));
    painter.drawRect(left_up, left_up, CHESSBOARD_LEN ,CHESSBOARD_LEN);

    // 水平线
    for(int i = 0 ; i < number ; i++)
    {

        painter.drawLine( LEFT_UP() , LEFT_UP() + i * L , LEFT_UP() + ( number - 1 ) * L , LEFT_UP() + i * L );
    }

    //垂直线
    for(int i = 0 ; i < number ; i++)
    {
        painter.drawLine( LEFT_UP() + i * L , LEFT_UP() ,  LEFT_UP() + i * L ,  LEFT_UP() + ( number - 1 ) * L );
    }
}

void GameWidget::drawWhite(QPainter &painter, double i, double j)
{

    double Size = SQUARE_LEN() * 0.7, Strength = Size / 4;

    double xi = LEFT_UP() + ( i - 0.35 ) * SQUARE_LEN();
    double yi = LEFT_UP() + ( j - 0.35 ) * SQUARE_LEN();

    painter.setPen(QPen(QColor(CUS_WHITE), Strength, Qt::SolidLine));
    painter.setBrush(QColor(BG_COLOR));

    painter.drawEllipse(xi, yi, Size, Size);
}

void GameWidget::drawBlack(QPainter &painter, double i, double j)
{

    double Size = SQUARE_LEN() * 0.7, Strength = Size / 4;
    double xi = LEFT_UP() + ( i - 0.35 ) * SQUARE_LEN();
    double yi = LEFT_UP() + ( j - 0.35 ) * SQUARE_LEN();

    painter.setPen(QPen(QColor(CUS_BLACK), Strength, Qt::SolidLine));
    painter.setBrush(QColor(BG_COLOR));

    painter.drawEllipse(xi, yi, Size, Size);
}

void GameWidget::drawDemo(QPainter &painter) // 绘画 FYH
{
    for(int i = 0; i < CHESSBOARD_SIZE; i++)
        for(int j = 0; j < CHESSBOARD_SIZE; j++)
        {
            if(judge->GridPoint(i, j)) return;
        }
    srand(time(0));

    int fyhBoard[50][50] = {};
    int Len = (CHESSBOARD_SIZE - 1) / 2;

    int op = 1 + (CHESSBOARD_SIZE % (Len + 1) == 0);

    for(int i = 0; i < Len; i++)
        fyhBoard[0][i] = fyhBoard[i][0] = 1, fyhBoard[i + Len + op][0] = fyhBoard[i + Len + op][Len - 1] = -1;
    for(int i = 0; i < Len; i++)
        fyhBoard[Len / 2][i] = 1, fyhBoard[Len / 2 + Len + op][i] = -1;

    for(int i = 0; i < Len / 2 ; i++)
        fyhBoard[i][CHESSBOARD_SIZE - Len + i] = fyhBoard[i][CHESSBOARD_SIZE - i - 1] = -1;

    for(int i = Len / 2; i < Len; i++)
        fyhBoard[i][CHESSBOARD_SIZE - Len / 2 - 1] = -1;

//    qDebug() << Len <<"\n";
    if(Len % 2 == 0)
        for(int i = Len / 2; i < Len; i++)
            fyhBoard[i][CHESSBOARD_SIZE - Len / 2] = -1;

    for(int i = 0; i < CHESSBOARD_SIZE; i++)
        for(int j = 0; j < CHESSBOARD_SIZE; j++)
        {
            if(!fyhBoard[j][i]) continue;
            if(fyhBoard[j][i] > 0)
                drawWhite(painter, i, j);
            else
                drawBlack(painter, i, j);
        }
    for(int i = 0; i < Len; i++)
        for(int j = 0; j < Len; j++) // 心形线
        {
            double x = (i - Len / 2.0) / Len * 2, y = (j + 1 - Len / 2.0) / Len * 2;
//            qDebug() << i << " " << j <<"->"<<x<<" "<<y<<" "<<fabs(pow((x * x + y * y - 1), 3) - x * x * y * y * y)<<"\n";
            if(fabs(pow((x * x + y * y - 0.4), 1) - 4 * x * x * y * y * y) <= 0.143 * 28 / CHESSBOARD_SIZE)
                drawBlack(painter, i + Len + op, (Len - j - 1) + Len + op);
            else
                drawWhite(painter, i + Len + op, (Len - j - 1) + Len + op);
        }
}

void GameWidget::playerTimeout() {if(judge->curPlayer >= 0) gameLose(1);}
void GameWidget::gameLose(int type)
{
    judge->curPlayerBak = judge->curPlayer;
    if(!type) sendMessage(1);
    else sendMessage(3);
    judge->curPlayer = -1;
}
void GameWidget::botTimeout() {if(judge->curPlayer >= 0) gameWin(1);}
void GameWidget::gameWin(int type)
{
    judge->curPlayerBak = judge->curPlayer;
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
 * type=6 : player resign
*/
void GameWidget::sendMessage(int type)
{
    // mess = new MessageBox(QString("qwq"), this);
    if(!judge->runMode)
    {
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
            case 5:
                mess = new MessageBox(QString("Sorry, You Resign!\n\nPlease restart."), 3000, this);
                break;
        }
    }
    else{
        switch(type)
        {
            case 2:
                mess = new MessageBox(QString("You cannot place a \npiece there!"), 2000, this);
                break;
            case 3:
            // qDebug() << judge->curPlayerBak;
                if(judge->curPlayerBak) mess = new MessageBox(QString("TIME'S UP!\n\nPlayer1 LOSE"), 3000, this);
                else mess = new MessageBox(QString("TIME'S UP!\n\nPlayer2 LOSE"), 3000, this);
                break;
            case 5:
            qDebug() << judge->curPlayerBak;
                if(judge->curPlayerBak) mess = new MessageBox(QString("Sorry, Player1 Resign!\n\nPlease restart."), 3000, this);
                else mess = new MessageBox(QString("Sorry, Player2 Resign!\n\nPlease restart."), 3000, this);
                break;
        }
    }
    mess->show();
}

void GameWidget::on_resignButton_clicked()
{
    qDebug() << judge->runMode;
    judge->curPlayerBak = judge->curPlayer;
    sendMessage(5);
    judge->curPlayer = -1;
}

void GameWidget::on_restartButton_clicked()
{
    emit restartSingal(0);
    judge->init();
    if(mess) mess->close();
    this->close();
}

void GameWidget::startTimer()
{
    timerForPlayer->start(PLAYER_TIMEOUT * 1000);
}
