#include "Widget/gamewidgetol.h"
#include "ui_gamewidget.h"
#include "ui_gamewidgetol.h"

/*
*   @file: gamewidgetol.cpp
*   @brief: 声明 GameWidgetOL 类，
*           维护联机对局过程中的棋盘状态
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*/

GameWidgetOL::GameWidgetOL(Judge *j, Bot *b, QWidget *parent) :
    GameWidget(j, b, parent),
    ui(new Ui::GameWidgetOL)
{
    ui->setupUi(this);
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // 布局
    GameWidget::ui->resignButton->setStyleSheet(ACCENT_COLOR);
    GameWidget::ui->restartButton->setStyleSheet(ACCENT_COLOR);
    ui->chatHistory->setReadOnly(true);
    ui->chatHistory->appendPlainText(QString("Feel free to chat..."));
    ui->sendButton->setStyleSheet(ACCENT_COLOR);
    ui->chatInput->setStyleSheet("color: rgb(96, 150, 180); background-color: #ecf9ff;"
                                 "border-width: 1px; border-style: solid;"
                                 "border-color: #ecf9ff #ecf9ff rgb(96, 150, 180) #ecf9ff;");
    ui->chatHistory->setStyleSheet("color: rgb(96, 150, 180); background-color: #ecf9ff;"
                                   "border-style: none;");
    GameWidget::ui->saveButton->setGeometry(QRect(QPoint(0, 0), QSize(0, 0)));
    GameWidget::ui->loadButton->setGeometry(QRect(QPoint(0, 0), QSize(0, 0)));

    columnX = judge->RIGHT_UP() + (WINDOW_WIDTH - judge->RIGHT_UP()) / 2;
    columnY = (double)WINDOW_HEIGHT / 20 * 7.5;
    buttonH = (double)WINDOW_HEIGHT / 31 * 2;
    buttonW = (double)WINDOW_HEIGHT / 31 * 6;
    int hisH = (double)WINDOW_HEIGHT / 31 * 6,
        hisW = (double)WINDOW_HEIGHT / 31 * 8;
    int sendH = (double)WINDOW_HEIGHT / 25,
        sendW = (double)WINDOW_HEIGHT / 31 * 2;
    int inputH = sendH,
        inputW = hisW - sendW;
    int X1 = columnX - hisW / 2,
        Y1 = WINDOW_HEIGHT - judge->LEFT_UP() - hisH - inputH;
    ui->chatHistory->setGeometry(QRect(QPoint(X1, Y1), QSize(hisW, hisH)));
    int X2 = X1,
        Y2 = Y1 + hisH;
    ui->chatInput->setGeometry(QRect(QPoint(X2, Y2), QSize(inputW, inputH)));
    X2 += inputW;
    ui->sendButton->setGeometry(QRect(QPoint(X2, Y2), QSize(sendW, sendH)));
    deltaY = (double)(Y1 - columnY - 15) / 2;
    int X3 = columnX - buttonW / 2,
        Y3 = columnY;
    GameWidget::ui->resignButton->setGeometry(QRect(QPoint(X3, Y3), QSize(buttonW, buttonH)));
    int X4 = X3,
        Y4 = Y3 + deltaY;
    GameWidget::ui->restartButton->setGeometry(QRect(QPoint(X4, Y4), QSize(buttonW, buttonH)));
    LOGO_X = columnX - logoBoardW / 2;
    LOGO_Y = judge->LEFT_UP();
    int TIME_X = columnX - 100,
        TIME_Y = (LOGO_Y + logoBoardH + columnY - 10) / 2;
    GameWidget::ui->TimeBar->setGeometry(QRect(QPoint(TIME_X, TIME_Y), QSize(200,15)));

    disconnect(timerForPlayer,&QTimer::timeout, this, &GameWidget::playerTimeout);
    connect(timerForPlayer, &QTimer::timeout, this, &GameWidgetOL::playerTimeout);
    connect(ui->chatInput, SIGNAL(returnPressed()), ui->sendButton,SIGNAL(clicked()), Qt::UniqueConnection);
    connect(judge, &Judge::GIVEUP_OP, this, &GameWidgetOL::remoteResign);
    connect(judge, &Judge::TIMEOUT_END_OP, this, [&](){gameLose(1);});
    connect(judge, &Judge::SUICIDE_END_OP, this, [&](){gameWin(1);});
    connect(judge, &Judge::CHAT_OP, this, [&](NetworkData d){
        QString s = "<" + judge->oppoOL + "> : \n" + d.data1;
        ui->chatHistory->appendPlainText(s);
    });
}
GameWidgetOL::~GameWidgetOL()
{
    delete ui;
}

void GameWidgetOL::takeTurn(int row, int col)
{
    timerForPlayer->stop();
    timerForBar->stop();
    judge->PlaceAPiece(row, col);
    if(judge->runMode == 2 || judge->runMode == 3) // 发送 MOVE_OP 以及处理 recData
        judge->send(NetworkData(OPCODE::MOVE_OP,
                                QString(QChar('A'+row))+QString(QChar('1'+col)), ""));
    startTimer();
}

void GameWidgetOL::playerTimeout()
{
    // 发送 TIMEOUT_END_OP
    if(!judge->curPlayer) // 当前是等待响应的一方，那么对手超时己方胜利
    {
        judge->send(NetworkData(OPCODE::TIMEOUT_END_OP, judge->usrnameOL, "Sorry you lose!"));
        gameWin(1);
    }
}
void GameWidgetOL::remoteResign()
{
    sendMessage(6);
    judge->curPlayerBak = judge->curPlayer;
    judge->curPlayer = -1;
    timerForPlayer->stop();
    timerForBar->stop();
}
void GameWidgetOL::sendMessage(int type)
{
    ItemVector step = judge->getStep();
    int steps=step.size();
    if(judge->loadState=='G')
        steps++;
    QString ssteps = QString::number(steps);
    clickToCloseMB(true);
    if(judge->runMode == 2 || judge->runMode == 3)
    {
        switch(type)
        {
        case 0:
            mess = new MessageBox(QString("You WIN!\n\n") + judge->oppoOL + QString("\ntime out."), 0, true, this);
            break;
        case 1:
            mess = new MessageBox(QString("TIME'S UP!\n\n") + judge->oppoOL + QString("\nwins."), 0, true, this);
            break;
        case 2:
            mess = new MessageBox(QString("You cannot place a \npiece there!"), 2000, false, this);
            break;
        case 5:
            mess = new MessageBox(QString("You resign!\n\n") + judge->oppoOL + QString("\nwins."), 0, true, this);
            break;
        case 6:
            mess = new MessageBox(QString("You WIN!\n\n") + judge->oppoOL + QString("\nresigns."), 0, true, this);
            break;
        case 7:
            mess = new MessageBox(QString("Please resign before\nrestarting the game."), 2000, false, this);
            break;
        }
    }
    mess->show();
}

// 按钮行为
void GameWidgetOL::on_sendButton_clicked()
{
    QString data = ui->chatInput->text();
    QString s = "<" + judge->usrnameOL + "> : \n" + data;
    ui->chatHistory->appendPlainText(s);
    ui->chatInput->setText("");
    judge->send(NetworkData(OPCODE::CHAT_OP, data, ""));
}
void GameWidgetOL::on_restartButton_clicked()
{
    // 不发送 LEAVE_OP，返回到主界面，相当于重置
    if(judge->curPlayer != -1) // 棋局尚在进行
    {
        sendMessage(7);
        return;
    }
    ui->chatHistory->clear();
    ui->chatHistory->appendPlainText(QString("Feel free to chat..."));
    GameWidget::on_restartButton_clicked();
}
void GameWidgetOL::on_resignButton_clicked()
{
    if(judge->curPlayer == -1) return;
    // 发送 GIVEUP_OP
    GameWidget::on_resignButton_clicked();
    judge->send(NetworkData(OPCODE::GIVEUP_OP, judge->usrnameOL, ""));
}
