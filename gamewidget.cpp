#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "judge.h"

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    setFixedSize(PIC_WIDTH, PIC_HEIGHT);
    // setMouseTracking(true);

    ui->restartButton->setStyleSheet(STRESS_COLOR);
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
    QPixmap chessboardImg;
    chessboardImg.load(":/map/chessboard.png");
    painter.drawPixmap(0, 0, PIC_WIDTH, PIC_HEIGHT, chessboardImg);

    drawDemo(painter);
    // 绘制棋子
    for(int i = 0; i < CHESS_BOARD_SIZE; i++)
        for(int j = 0; j < CHESS_BOARD_SIZE; j++)
        {
            if(this->judge->board[i][j] == 1)
            {
                drawWhite(painter, i, j);
            }
            if(this->judge->board[i][j] == -1)
            {
                drawBlack(painter, i, j);
            }

        }
}

void GameWidget::drawWhite(QPainter &painter, double i, double j)
{
    double Sx = (double)PIC_WIDTH / 1282 * 112, Sy = Sx;
    double Dx = (double)PIC_WIDTH / 1282 * 79.7, Dy = Dx;
    double px = Sx + i * Dx, py = Sy + j * Dy;
    double Size = PIC_WIDTH / 22, Strength = Size / 4;

    painter.setPen(QPen(QColor(CUS_WHITE), Strength, Qt::SolidLine));
    painter.setBrush(QColor(BG_COLOR));
    painter.drawEllipse(px, py, Size, Size);
}

void GameWidget::drawBlack(QPainter &painter, double i, double j)
{
    double Sx = (double)PIC_WIDTH / 1282 * 112, Sy = Sx;
    double Dx = (double)PIC_WIDTH / 1282 * 79.7, Dy = Dx;
    double px = Sx + i * Dx, py = Sy + j * Dy;
    double Size = PIC_WIDTH / 22, Strength = Size / 4;

    painter.setPen(QPen(QColor(CUS_BLACK), Strength, Qt::SolidLine));
    painter.setBrush(QColor(BG_COLOR));
    painter.drawEllipse(px, py, Size, Size);
}

void GameWidget::drawDemo(QPainter &painter)
{
    srand(time(0));
    for(int i = 0; i < CHESS_BOARD_SIZE; i++)
        for(int j = 0; j < CHESS_BOARD_SIZE; j++)
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

    double Sx = (double)PIC_WIDTH / 1282 * 112, Sy = Sx;
    double Dx = (double)PIC_WIDTH / 1282 * 79.7, Dy = Dx;

    for(int i = 0; i < CHESS_BOARD_SIZE; i++)
        for(int j = 0; j < CHESS_BOARD_SIZE; j++){
            double x1 = Sx + i * Dx, y1 = Sy + j * Dy;
            double x2 = x1 + 0.8 * Dx, y2 = y1 + 0.8 * Dy;
            //qDebug() << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << '\n';
            if(x > x1 && x < x2 && y > y1 && y < y2){
                judge->clickedRow = i;
                judge->clickedColumn = j;
                judge->board[i][j] = judge->AI_role;
            }
        }

    update();
}

void GameWidget::on_restartButton_clicked()
{
    emit restartSingal(0);
    judge->init();
    this->close();
}
