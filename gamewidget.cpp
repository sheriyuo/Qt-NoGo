/*
* 默认大小 ： 1024 * 768
* 棋盘大小 ： 702 * 702
*
*
*/
#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "judge.h"
#include <math.h>

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    setFixedSize(PIC_WIDTH, PIC_HEIGHT);
    // setMouseTracking(true);

    ui->restartButton->setStyleSheet(ACCENT_COLOR);
}

GameWidget::~GameWidget()
{
    delete ui;
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    // 绘制棋盘
    drawChessboard(painter);

//drawDemo(painter);
    // 绘制棋子
    for(int i = 0; i < CHESSBOARD_SIZE; i++)
        for(int j = 0; j < CHESSBOARD_SIZE; j++)
        {
            if(this->judge->board[i][j] == -1)
            {
                drawWhite(painter, i, j);
            }
            if(this->judge->board[i][j] == 1)
            {
                drawBlack(painter, i, j);
            }

        }
}

void GameWidget :: drawChessboard(QPainter &painter)
{

    int number=CHESSBOARD_SIZE;
    int left_up,L;
    left_up=(PIC_HEIGHT-CHESSBOARD_LEN)/2;
    L=SQUARE_LEN;

    //背景
    painter.setBrush(QColor(BG_COLOR));
    painter.drawRect(0, 0, PIC_WIDTH, PIC_HEIGHT);

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
            if(judge->board[i][j]) return;
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

// 监听鼠标点击实现落子
void GameWidget::mousePressEvent(QMouseEvent *event)
{
    double x = event->position().x(), y = event->position().y();

    qDebug() << "clicked pos : " << x << ' ' << y << '\n';
    qDebug() <<LEFT_UP;

    int row = 0,column = 0;
    double checklen = SQUARE_LEN / 2;

    for(int i = 1 ; i <=  CHESSBOARD_SIZE ; i++)
    {
        int xi = LEFT_UP + (i-1) * SQUARE_LEN;
        if(abs ( x - xi ) < checklen)
        {
            judge->clickedRow = i-1;
            row = i;
            break;
        }
    }

    for(int i = 1 ; i <= CHESSBOARD_SIZE ; i++)
    {
        int yi = LEFT_UP + ( i - 1 ) * SQUARE_LEN;
        if(abs( y - yi ) < checklen)
        {
            judge->clickedColumn = i-1;
            column = i;
            break;
        }
    }

    judge->board[row-1][column-1] = judge->playerRole;
    update();

}

void GameWidget::on_restartButton_clicked()
{
    emit restartSingal(0);
    judge->init();
    this->close();
}
