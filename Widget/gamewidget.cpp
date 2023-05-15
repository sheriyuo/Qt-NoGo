#include "Widget/gamewidget.h"
#include "ui_gamewidget.h"

/*
*   @file: gamewidget.cpp
*   @brief: 声明 GameWidget 类，
*           维护对局过程中的棋盘状态，控制信号槽及文件读写
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*/

GameWidget::GameWidget(Judge *j, Bot *b, QWidget *parent) :
    QWidget(parent),
    judge(j),
    bot(b),
    ui(new Ui::GameWidget)
{
    columnX = judge->RIGHT_UP() + (WINDOW_WIDTH - judge->RIGHT_UP()) / 2;
    columnY = (double)WINDOW_HEIGHT / 20 * 9;
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
    logoBoardW = (WINDOW_WIDTH - judge->RIGHT_UP()) * 0.65; // 等比缩放
    logoBoardH = (double)logoImg.height() * logoBoardW / logoImg.width();
    LOGO_X = columnX - logoBoardW / 2;
    LOGO_Y = judge->LEFT_UP() + 15;

    //设置 timebar 的位置
    int TIME_X = columnX - 100,
        TIME_Y = (LOGO_Y + logoBoardH + columnY) / 2 - 15;
    ui->TimeBar->setGeometry(QRect(QPoint(TIME_X, TIME_Y), QSize(200,15)));

    // 链接计时器
    timerForPlayer = new QTimer;
    timerForBar = new QTimer;

    connect(this, &GameWidget::mousePress, this, [&](){clickToCloseMB();});
    connect(timerForPlayer,&QTimer::timeout, this, &GameWidget::playerTimeout);
    connect(timerForBar,&QTimer::timeout,this,&GameWidget::updateBar);
    connect(bot, &Bot::timeout, this, &GameWidget::botTimeout);
    connect(ui->resignButton, &QPushButton::clicked, this, &GameWidget::on_resignButton_clicked);
    connect(ui->restartButton, &QPushButton::clicked, this, &GameWidget::on_restartButton_clicked);
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
    double checklen = judge->SQUARE_LEN() / 2;
    for(int i = 1 ; i <= judge->CHESSBOARD_SIZE ; i++)
    {
        int xi = judge->LEFT_UP() + (i-1) * judge->SQUARE_LEN();
        if(abs ( x - xi ) < checklen)
        {
            column = i;
            break;
        }
    }
    for(int i = 1 ; i <= judge->CHESSBOARD_SIZE ; i++)
    {
        int yi = judge->LEFT_UP() + ( i - 1 ) * judge->SQUARE_LEN();
        if(abs( y - yi ) < checklen)
        {
            row = i;
            break;
        }
    }

    // 此处实现轮流下棋
    if(row - 1 < 0 || column - 1 < 0 || row - 1 >= judge->CHESSBOARD_SIZE || column - 1 >= judge->CHESSBOARD_SIZE)
        return;
    if(judge->CheckVaild(row-1, column-1)) takeTurn(row-1, column-1);
    else sendMessage(2);
}
void GameWidget::takeTurn(int row, int col)
{
    timerForPlayer->stop();
    timerForBar->stop();
    judge->PlaceAPiece(row, col);
    if(!judge->runMode) emit turnForBot();
    startTimer();
}

void GameWidget::updateCB() {repaint();}
void GameWidget::clickToCloseMB(bool force){
    if(mess){
        if(force){
            mess->timeUpClose();
            mess = nullptr;
        }
        else if(mess->clickToClose()){
            mess = nullptr;
        }
    }
}

void GameWidget::setColorForBar()
{
    if(judge->curPlayer == -1) return;
    if(judge->CurColor() == -1) // White
    {
        ui->TimeBar->setStyleSheet("QProgressBar{"
                                   "background:rgb(186,215,233);"
                                   "color:white;"
                                   "border-radius:5px;}"
                                   "QProgressBar::chunk{"
                                   "border-radius:5px;"
                                   "background: #7acbf5}");
    }
    else // Black
    {
        ui->TimeBar->setStyleSheet("QProgressBar{"
                                   "background:rgb(186,215,233);"
                                   "color:white;"
                                   "border-radius:5px;}"
                                   "QProgressBar::chunk{"
                                   "border-radius:5px;"
                                   "background: #EAACB8}");
    }
}
void GameWidget::updateBar()
{
    double value;
    value=(clock()-basetime);
    ui->TimeBar->setValue(1000-value/PLAYER_TIMEOUT);
    repaint();
}

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
                //if(xxx==i&&yyy==j)
                {
                    drawWhite(painter, i, j);
                    //drawheat(painter,i,j);

                }
                //else
                    //drawWhite(painter, i, j);
            }
            if(this->judge->GridPoint(i, j) == 1)
            {
                //if(xxx==i&&yyy==j)
                {
                    drawBlack(painter, i, j);
                    //drawheat(painter,i,j);

                }
                //else
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
        painter.drawLine( judge->LEFT_UP() , judge->LEFT_UP() + i * L , judge->LEFT_UP() + ( number - 1 ) * L , judge->LEFT_UP() + i * L );

    //垂直线
    for(int i = 0 ; i < number ; i++)
        painter.drawLine( judge->LEFT_UP() + i * L , judge->LEFT_UP() ,  judge->LEFT_UP() + i * L ,  judge->LEFT_UP() + ( number - 1 ) * L );
}

void GameWidget::drawWhite(QPainter &painter, double i, double j)
{

    double Size = judge->SQUARE_LEN() * 0.7, Strength = Size / 4;

    double xi = judge->LEFT_UP() + ( j - 0.35 ) * judge->SQUARE_LEN();
    double yi = judge->LEFT_UP() + ( i - 0.35 ) * judge->SQUARE_LEN();

    painter.setPen(QPen(QColor(CUS_WHITE), Strength, Qt::SolidLine));
    painter.setBrush(QColor(BG_COLOR));

    painter.drawEllipse(xi, yi, Size, Size);

    int xxx=-100;
    int yyy=-100;
    ItemVector step = judge->getStep();
    if(!step.empty())
    {
        xxx=step[step.size()-1].first;
        yyy=step[step.size()-1].second;
    }
    if(xxx==i&&yyy==j)
    {
        double Size = judge->SQUARE_LEN() * 0.18, Strength = Size / 4;

        double xi = judge->LEFT_UP() + ( j - 0.09 ) * judge->SQUARE_LEN();
        double yi = judge->LEFT_UP() + ( i - 0.09 ) * judge->SQUARE_LEN();

        painter.setPen(QPen(QColor(CUS_WHITE), Strength, Qt::SolidLine));
        painter.setBrush(QColor(CUS_WHITE));

        painter.drawEllipse(xi, yi, Size, Size);
    }
}

void GameWidget::drawBlack(QPainter &painter, double i, double j)
{

    double Size = judge->SQUARE_LEN() * 0.7, Strength = Size / 4;
    double xi = judge->LEFT_UP() + ( j - 0.35 ) * judge->SQUARE_LEN();
    double yi = judge->LEFT_UP() + ( i - 0.35 ) * judge->SQUARE_LEN();

    painter.setPen(QPen(QColor(CUS_BLACK), Strength, Qt::SolidLine));
    painter.setBrush(QColor(BG_COLOR));

    painter.drawEllipse(xi, yi, Size, Size);

    int xxx=-100;
    int yyy=-100;
    ItemVector step = judge->getStep();
    if(!step.empty())
    {
        xxx=step[step.size()-1].first;
        yyy=step[step.size()-1].second;
    }
    if(xxx==i&&yyy==j)
    {
        double Size = judge->SQUARE_LEN() * 0.18, Strength = Size / 4;

        double xi = judge->LEFT_UP() + ( j - 0.09 ) * judge->SQUARE_LEN();
        double yi = judge->LEFT_UP() + ( i - 0.09 ) * judge->SQUARE_LEN();

        painter.setPen(QPen(QColor(CUS_BLACK), Strength, Qt::SolidLine));
        painter.setBrush(QColor(CUS_BLACK));

        painter.drawEllipse(xi, yi, Size, Size);
    }
}
void GameWidget::drawheat(QPainter &painter,double i, double j)
{

    double Size = judge->SQUARE_LEN() * 0.2, Strength = Size / 4;
    double xi = judge->LEFT_UP() + ( j - 0.1 ) * judge->SQUARE_LEN();
    double yi = judge->LEFT_UP() + ( i - 0.1 ) * judge->SQUARE_LEN();

    int color=judge->CurColor();
    if(color==-1)
        painter.setPen(QPen(QColor(CUS_WHITE), Strength, Qt::SolidLine));
    else
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
            if(!fyhBoard[i][j]) continue;
            if(fyhBoard[i][j] > 0)
                drawWhite(painter, i, j);
            else
                drawBlack(painter, i, j);
        }
    for(int i = 0; i < Len; i++)
        for(int j = 0; j < Len; j++)  // 心形线
        {
            double x = (i - Len / 2.0) / Len * 2, y = (j + 1 - Len / 2.0) / Len * 2;
            if(fabs(pow((x * x + y * y - 0.4), 1) - 4 * x * x * y * y * y) <= 0.143 * 28 / judge->CHESSBOARD_SIZE)
                drawBlack(painter, (Len - j - 1) + Len + op, i + Len + op);
            else
                drawWhite(painter, (Len - j - 1) + Len + op, i + Len + op);
        }
}

// 判胜负与计时器相关
void GameWidget::gameLose(int type)
{
    judge->curPlayerBak = judge->curPlayer;
    if(type) sendMessage(1);
    else sendMessage(3);
    judge->curPlayer = -1;
    judge->loadState = 'L';
}
void GameWidget::gameWin(int type)
{
    judge->curPlayerBak = judge->curPlayer;
    if(type) sendMessage(0);
    else sendMessage(4);
    judge->curPlayer = -1;
    judge->loadState = 'W';
}
void GameWidget::startTimer() {
    setColorForBar();
    timerForPlayer->stop();
    timerForBar->stop();
    if(judge->curPlayer >= 0)
    {
        timerForPlayer->start(PLAYER_TIMEOUT * 1000);
        timerForBar->start(100);
        basetime=clock();
    }
}
void GameWidget::botTimeout() {if(judge->curPlayer >= 0) gameWin(0);}
void GameWidget::playerTimeout() {if(judge->curPlayer >= 0) gameLose(0);}

void GameWidget::sendMessage(int type)
{
    ItemVector step = judge->getStep();
    int steps=step.size();
    if(judge->loadState=='G')
            steps++;
    QString ssteps = QString::number(steps);
    clickToCloseMB(true);
    if(judge->runMode == 0)
    {
        switch(type)
        {
            case 2:
                mess = new MessageBox(QString("You cannot place a \npiece there!"), 2000, false, this);
                break;
            case 3:
                mess = new MessageBox(QString("TIME'S UP! You LOSE!")+QString("\n\nTotal step : ")+ssteps, 0, true, this);
                break;
            case 4:
                mess = new MessageBox(QString("Bot failed to make\na move.\nYou WIN"), 0, true, this);
                break;
            case 5:
                mess = new MessageBox(QString("You Resign!")+QString("\n\nTotal step : ")+ssteps, 0, true, this);
                break;
        }
    }
    if(judge->runMode == 1)
    {
        switch(type)
        {
            case 2:
                mess = new MessageBox(QString("You cannot place a \npiece there!"), 2000, false, this);
                break;
            case 3:
                if(judge->curPlayerBak) mess = new MessageBox(QString("TIME'S UP! Player1 LOSE!>")+QString("\n\nTotal step : "+ssteps), 0, true, this);
                else mess = new MessageBox(QString("TIME'S UP! Player2 LOSE!")+QString("\n\nTotal step : "+ssteps), 0, true, this);
                break;
            case 5:
                if(judge->curPlayerBak) mess = new MessageBox(QString("Player1 Resign!")+QString("\n\nTotal step : "+ssteps), 0, true, this);
                else mess = new MessageBox(QString("Player2 Resign!")+QString("\n\nTotal step : ")+ssteps, 0, true, this);
                break;
        }
    }
    mess->show();
}

// 定义按钮行为
void GameWidget::on_restartButton_clicked()
{
    clickToCloseMB(true);
    this->close();

    timerForPlayer->stop();
    timerForBar->stop();
    emit restartSingal(0);
    judge->init();
}
void GameWidget::on_resignButton_clicked()
{
    qDebug() << "OFF resign";
    if(judge->curPlayer == -1) return;
    sendMessage(5);
    judge->curPlayerBak = judge->curPlayer;
    judge->curPlayer = -1;
    judge->loadState = 'G';
    timerForPlayer->stop();
    timerForBar->stop();
}

// 存档读档相关编码
void GameWidget::dataToString()
{
    std::memset(dataStr,0,sizeof(dataStr));
    int len = 0;
    dataStr[len++] = judge->playerRole + '1', dataStr[len++] = ' ';
    dataStr[len++] = judge->runMode + '0', dataStr[len++] = ' ';

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

    char dataState = judge->loadState;
    if(dataState)
        dataStr[len++] = dataState;
    else
        dataStr[--len] = 0;
}

void GameWidget::stringToData()
{
    dataVec.clear();
    int len = strlen(dataStr);
    strState = 0;
    for(int i = 4; i < len; i += 3)
    {
        if(i == len - 1)
        {
            strState = dataStr[i];
            break;
        }
        int x = dataStr[i] - 'A', y = dataStr[i+1] - '1';
        dataVec.push_back(Point(x, y));
    }
}

// 读档按钮信号
void GameWidget::on_loadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Load Data"), "", tr("DATA (*.dat)"));
    // QFileDialog 读取文件 fileName

    if (fileName.isEmpty())
        return;
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) // 无法读入时报错
        {
            QMessageBox::information(nullptr, tr("Unable to open file"),file.errorString());
            return;
        }

        QByteArray fileData = file.readAll();

        memset(dataStr, 0, sizeof(dataStr)); // 先清零不然 copy 位数小于时会更新不了后面的
        memcpy(dataStr, fileData, fileData.size());
        // QFile 直接读入为 QByteArray 再转为 char*
    }

    stringToData();
    judge->updateStep(dataStr[0] - '1', dataStr[2] - '0', dataVec, strState);
    // 理论上，没有判断非法 .dat

    startTimer(); // 重置计时器

    if(judge->loadState) // 是否为终局
    {
        if(judge->loadState == 'W')
            gameWin(judge->runMode);
        if(judge->loadState == 'L')
            gameLose(judge->runMode);
        if(judge->loadState == 'G')
            on_resignButton_clicked();
    }

    // 重新开始游戏 judge
}
// 存档按钮信号
void GameWidget::on_saveButton_clicked()
{
    dataToString();
    QString fileName = QFileDialog::getSaveFileName(nullptr, tr("Save Data"), "", tr("DATA (*.dat)"));
    // QFileDialog 读取文件 fileName

    if (fileName.isEmpty())
        return;
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) // 无法覆写时报错
        {
            QMessageBox::information(nullptr, tr("Unable to open file"),file.errorString());
            return;
        }
        file.write(dataStr); // QFile 直接读写文件
        file.close();
    }
}
