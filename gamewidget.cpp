#include "gamewidget.h"
#include "ui_gamewidget.h"

/* 玩家的计时器
将 bot 的计时器内置于 class Bot 中 */
static QTimer *timerForPlayer;
static QTimer *timerForBar;
static time_t basetime;
// static QTimer *timerForBot;

static int deltaY, LOGO_X, LOGO_Y, logoBoardW, logoBoardH;

GameWidget::GameWidget(Judge *j, Bot *b, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget),
    judge(j),
    bot(b)
{
    columnX = judge->RIGHT_UP() + (WINDOW_WIDTH - judge->RIGHT_UP()) / 2;
    columnY = (double)WINDOW_HEIGHT / 20 * 10;
    // 右边侧栏位置中线
    buttonH = (double)WINDOW_HEIGHT / 31 * 2;
    buttonW = (double)WINDOW_HEIGHT / 31 * 6;
    // 右边按钮大小

    ui->setupUi(this);
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    deltaY = (double)(judge->RIGHT_UP() - columnY) / 4;
    // 设置按钮样式，无边框在 ui 文件中设置
    ui->loadButton->setStyleSheet(ACCENT_COLOR); // 文字颜色
    int X1 = columnX - buttonW / 2,
        Y1 = columnY - buttonH / 2;
    ui->loadButton->setGeometry(QRect(QPoint(X1, Y1), QSize(buttonW, buttonH))); // 位置

    ui->saveButton->setStyleSheet(ACCENT_COLOR);
    int X2 = X1,
        Y2 = Y1 + deltaY;
    ui->saveButton->setGeometry(QRect(QPoint(X2, Y2), QSize(buttonW, buttonH)));

    ui->resignButton->setStyleSheet(ACCENT_COLOR);
    int X3 = X1,
        Y3 = Y2 + deltaY;
    ui->resignButton->setGeometry(QRect(QPoint(X3, Y3), QSize(buttonW, buttonH)));

    ui->restartButton->setStyleSheet(ACCENT_COLOR);
    int X4 = X1,
        Y4 = Y3 + deltaY;
    ui->restartButton->setGeometry(QRect(QPoint(X4, Y4), QSize(buttonW, buttonH)));

    // 设置 logo 的位置
    logoImg.load(":/img/logo.png");
    logoBoardW = (WINDOW_WIDTH - judge->RIGHT_UP()) * 0.8; // 等比缩放
    logoBoardH = (double)logoImg.height() * logoBoardW / logoImg.width();
    LOGO_X = columnX - logoBoardW / 2;
    LOGO_Y = columnY - 2.5 * deltaY - logoBoardH / 2;

    //设置 timebar 的位置
    int TIME_X = columnX - 100,
        TIME_Y = (LOGO_Y + logoBoardH / 2 + columnY) / 2;
    ui->TimeBar->setGeometry(QRect(QPoint(TIME_X, TIME_Y), QSize(200,15)));

    // 链接计时器
    timerForPlayer = new QTimer;
    timerForBar = new QTimer;
    connect(timerForPlayer,&QTimer::timeout, this, &GameWidget::playerTimeout);
    connect(timerForBar,&QTimer::timeout,this,&GameWidget::updateBar);
    connect(bot, &Bot::timeout, this, &GameWidget::botTimeout);
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
    if(!judge->curPlayer) return; // 判断当前局

    double x = event->position().x(), y = event->position().y();
    int row = 0,column = 0;
    double checklen = judge->SQUARE_LEN() / 2;
    for(int i = 1 ; i <= judge->CHESSBOARD_SIZE ; i++)
    {
        int xi = judge->LEFT_UP() + (i-1) * judge->SQUARE_LEN();
        if(abs ( x - xi ) < checklen)
        {
            row = i;
            break;
        }
    }
    for(int i = 1 ; i <= judge->CHESSBOARD_SIZE ; i++)
    {
        int yi = judge->LEFT_UP() + ( i - 1 ) * judge->SQUARE_LEN();
        if(abs( y - yi ) < checklen)
        {
            column = i;
            break;
        }
    }

    // 此处实现轮流下棋
    if(row - 1 < 0 || column - 1 < 0 || row - 1 >= judge->CHESSBOARD_SIZE || column - 1 >= judge->CHESSBOARD_SIZE)
        return;
    if(judge->CheckVaild(row-1, column-1)){
        timerForPlayer->stop();
        timerForBar->stop();
        judge->PlaceAPiece(row-1, column-1);

        if(!judge->runMode)
        {
            emit turnForBot();
        }
        if(judge->runMode == 1)
        {
            timerForPlayer->start(PLAYER_TIMEOUT * 1000);
            timerForBar->start(100);
            basetime=clock();
        }
        if(judge->runMode == 2 || judge->runMode == 3)
        {
            // 发送 MoveOP 以及处理 recData
        }
    }
    else
    {
        sendMessage(2);
    }
}
void GameWidget::updateCB() {repaint();}

void GameWidget::updateBar()
{
    double value;
    value=(clock()-basetime);
    ui->TimeBar->setValue(1000-value/PLAYER_TIMEOUT);
    repaint();
}
void GameWidget::closeMB() {if(mess) mess->close();}

void GameWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    drawChessboard(painter);
    painter.drawPixmap(LOGO_X, LOGO_Y, logoBoardW, logoBoardH, logoImg);
    // 绘制 demo
    if(judge->CHESSBOARD_SIZE == 28)
        drawDemo(painter);

    // 绘制棋子
    for(int i = 0; i < judge->CHESSBOARD_SIZE; i++)
        for(int j = 0; j < judge->CHESSBOARD_SIZE; j++)
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

    int number=judge->CHESSBOARD_SIZE;
    double left_up,L;
    left_up=(WINDOW_HEIGHT-CHESSBOARD_LEN)/2;
    L=judge->SQUARE_LEN();

    //背景
    painter.setBrush(QColor(BG_COLOR));
    painter.drawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    painter.setPen(QPen(QColor(GRID_COLOR), judge->GRID_THICKNESS(), Qt::SolidLine));
    painter.drawRect(left_up, left_up, CHESSBOARD_LEN ,CHESSBOARD_LEN);

    // 水平线
    for(int i = 0 ; i < number ; i++)
    {

        painter.drawLine( judge->LEFT_UP() , judge->LEFT_UP() + i * L , judge->LEFT_UP() + ( number - 1 ) * L , judge->LEFT_UP() + i * L );
    }

    //垂直线
    for(int i = 0 ; i < number ; i++)
    {
        painter.drawLine( judge->LEFT_UP() + i * L , judge->LEFT_UP() ,  judge->LEFT_UP() + i * L ,  judge->LEFT_UP() + ( number - 1 ) * L );
    }
}

void GameWidget::drawWhite(QPainter &painter, double i, double j)
{

    double Size = judge->SQUARE_LEN() * 0.7, Strength = Size / 4;

    double xi = judge->LEFT_UP() + ( i - 0.35 ) * judge->SQUARE_LEN();
    double yi = judge->LEFT_UP() + ( j - 0.35 ) * judge->SQUARE_LEN();

    painter.setPen(QPen(QColor(CUS_WHITE), Strength, Qt::SolidLine));
    painter.setBrush(QColor(BG_COLOR));

    painter.drawEllipse(xi, yi, Size, Size);
}

void GameWidget::drawBlack(QPainter &painter, double i, double j)
{

    double Size = judge->SQUARE_LEN() * 0.7, Strength = Size / 4;
    double xi = judge->LEFT_UP() + ( i - 0.35 ) * judge->SQUARE_LEN();
    double yi = judge->LEFT_UP() + ( j - 0.35 ) * judge->SQUARE_LEN();

    painter.setPen(QPen(QColor(CUS_BLACK), Strength, Qt::SolidLine));
    painter.setBrush(QColor(BG_COLOR));

    painter.drawEllipse(xi, yi, Size, Size);
}

void GameWidget::drawDemo(QPainter &painter) // 绘画 FYH
{
    for(int i = 0; i < judge->CHESSBOARD_SIZE; i++)
        for(int j = 0; j < judge->CHESSBOARD_SIZE; j++)
        {
            if(judge->GridPoint(i, j)) return;
        }
    srand(time(0));

    int fyhBoard[50][50] = {};
    int Len = (judge->CHESSBOARD_SIZE - 1) / 2;

    int op = 1 + (judge->CHESSBOARD_SIZE % (Len + 1) == 0);

    for(int i = 0; i < Len; i++)
        fyhBoard[0][i] = fyhBoard[i][0] = 1, fyhBoard[i + Len + op][0] = fyhBoard[i + Len + op][Len - 1] = -1;
    for(int i = 0; i < Len; i++)
        fyhBoard[Len / 2][i] = 1, fyhBoard[Len / 2 + Len + op][i] = -1;

    for(int i = 0; i < Len / 2 ; i++)
        fyhBoard[i][judge->CHESSBOARD_SIZE - Len + i] = fyhBoard[i][judge->CHESSBOARD_SIZE - i - 1] = -1;

    for(int i = Len / 2; i < Len; i++)
        fyhBoard[i][judge->CHESSBOARD_SIZE - Len / 2 - 1] = -1;

    if(Len % 2 == 0)
        for(int i = Len / 2; i < Len; i++)
            fyhBoard[i][judge->CHESSBOARD_SIZE - Len / 2] = -1;

    for(int i = 0; i < judge->CHESSBOARD_SIZE; i++)
        for(int j = 0; j < judge->CHESSBOARD_SIZE; j++)
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
            if(fabs(pow((x * x + y * y - 0.4), 1) - 4 * x * x * y * y * y) <= 0.143 * 28 / judge->CHESSBOARD_SIZE)
                drawBlack(painter, i + Len + op, (Len - j - 1) + Len + op);
            else
                drawWhite(painter, i + Len + op, (Len - j - 1) + Len + op);
        }
}

// 判胜负与计时器相关
void GameWidget::gameLose(int type)
{
    judge->curPlayerBak = judge->curPlayer;
    if(!type) sendMessage(1);
    else sendMessage(3);
    judge->curPlayer = -1;
}
void GameWidget::gameWin(int type)
{
    judge->curPlayerBak = judge->curPlayer;
    if(!type) sendMessage(0);
    else sendMessage(4);
    judge->curPlayer = -1;
}
void GameWidget::startTimer() {
    timerForPlayer->start(PLAYER_TIMEOUT * 1000);
    timerForBar->start(100);
    basetime=clock();
}
void GameWidget::playerTimeout() {if(judge->curPlayer >= 0) gameLose(1);}
void GameWidget::botTimeout() {if(judge->curPlayer >= 0) gameWin(1);}


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
    if(mess){
        mess->close();
        mess = nullptr;
    }
    if(!judge->runMode)
    {
        switch(type)
        {
            /*case 0:
                mess = new MessageBox(QString("Congratulations!\n\nYou WIN"), 0, this);
                break;
            case 1:
                mess = new MessageBox(QString("Sorry!\n\nYou LOSE"), 0, this);
                break;*/
            case 2:
                mess = new MessageBox(QString("You cannot place a \npiece there!"), 2000, this);
                break;
            case 3:
                mess = new MessageBox(QString("TIME'S UP! You LOSE!\n\n<detailed info>"), 0, this);
                break;
            case 4:
                mess = new MessageBox(QString("Bot failed to make\na move.\nYou WIN"), 0, this);
                break;
            case 5:
                mess = new MessageBox(QString("You Resign!\n\n<detailed info>"), 0, this);
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
                if(judge->curPlayerBak) mess = new MessageBox(QString("TIME'S UP! Player1 LOSE!\n\n<detailed info>"), 0, this);
                else mess = new MessageBox(QString("TIME'S UP! Player2 LOSE!\n\n<detailed info>"), 0, this);
                break;
            case 5:
                if(judge->curPlayerBak) mess = new MessageBox(QString("Player1 Resign!\n\n<detailed info>"), 0, this);
                else mess = new MessageBox(QString("Player2 Resign!\n\n<detailed info>"), 0, this);
                break;
        }
    }
    mess->show();
}

// 定义按钮行为
void GameWidget::on_restartButton_clicked()
{
    if(mess){
        mess->close();
        mess = nullptr;
    }
    this->close();

    timerForPlayer->stop();
    timerForBar->stop();
    emit restartSingal(0);
    judge->init();
}
void GameWidget::on_resignButton_clicked()
{
    sendMessage(5);
    judge->curPlayerBak = judge->curPlayer;
    judge->curPlayer = -1;
    timerForPlayer->stop();
    timerForBar->stop();
}

// 存档读档相关编码
void GameWidget::dataToString()
{
    std::memset(dataStr,0,sizeof(dataStr));
    int len = 0;
    ItemVector step = judge->getStep(); // 读取 savedStep
    for(Item cur : step)
    {
        if(cur.first < 0)
        {
            dataStr[len++] = 'G';
            break;
        }
        char x = cur.first + 'A', y = cur.second + '1';
        dataStr[len++] = x, dataStr[len++] = y, dataStr[len++] = ' ';
    }
}

void GameWidget::stringToData()
{
    dataVec.clear();
    int len = strlen(dataStr);
    for(int i = 0; i < len; i += 3)
    {
        if(i == len - 1)
        {
            // 'G' 我还没写
            break;
        }
        int x = dataStr[i] - 'A', y = dataStr[i+1] - '1';
        dataVec.push_back(Point(x, y));
    }
}

// 存档读档按钮信号
void GameWidget::on_loadButton_clicked()
{
    // 文件操作我还没写

    stringToData();
    judge->updateStep(dataVec);
    startTimer();
}
void GameWidget::on_saveButton_clicked()
{
    dataToString();

    // 文件操作我还没写
}
