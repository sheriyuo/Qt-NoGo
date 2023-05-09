#include "DialogBox/settingdialog.h"
#include "ui_settingdialog.h"

/*
*   @file: settingdialog.h
*   @brief: 声明 SettingDialog 类，
*           实现图形化更改游戏选项
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*/

SettingDialog::SettingDialog(Judge *j, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog),
    judge(j)
{
    ui->setupUi(this);
    WIDTH = WINDOW_WIDTH * 3 / 5, HEIGHT = WINDOW_HEIGHT / 2;
    setFixedSize(WIDTH, HEIGHT);
    setWindowFlags(Qt::Window|Qt::WindowTitleHint);

    ui->closeBtn->setStyleSheet(ACCENT_COLOR);
    int H = (double)HEIGHT / 10,
        W = (double)HEIGHT / 5;
    int X = (double)WIDTH / 2 - (double)HEIGHT / 10,
        Y = (double)HEIGHT / 20 * 17.5 - (double)HEIGHT / 20;
    ui->closeBtn->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));

    ui->gamemode->setStyleSheet(ACCENT_COLOR);
    ui->gamemodeCB->setStyleSheet(ACCENT_COLOR);
    ui->chessbd->setStyleSheet(ACCENT_COLOR);
    ui->chessbdCB->setStyleSheet(ACCENT_COLOR);
        H = (double)HEIGHT / 10,
        W = (double)HEIGHT / 2.5;
        X = (double)WIDTH / 6 - (double)HEIGHT / 6,
        Y = (double)HEIGHT / 20 * 3 - (double)HEIGHT / 20;
    ui->gamemode->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 5 - (double)HEIGHT / 20;
    ui->gamemodeCB->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 8 - (double)HEIGHT / 20;
    ui->chessbd->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 10 - (double)HEIGHT / 20;
    ui->chessbdCB->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));

    ui->IPtitle->setStyleSheet(ACCENT_COLOR);
    ui->IPinput->setStyleSheet(ACCENT_COLOR);
    ui->IPinput->setText(judge->IP);
    ui->IPinput->setAlignment(Qt::AlignVCenter);
    ui->PORTtitle->setStyleSheet(ACCENT_COLOR);
    ui->PORTinput->setStyleSheet(ACCENT_COLOR);
    ui->PORTinput->setText(QString::number(judge->PORT));
    ui->PORTinput->setAlignment(Qt::AlignVCenter);
        X = (double)WIDTH * 3 / 6 - (double)HEIGHT / 5,
        Y = (double)HEIGHT / 20 * 3 - (double)HEIGHT / 20;
    ui->IPtitle->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 5 - (double)HEIGHT / 20;
    ui->IPinput->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 8 - (double)HEIGHT / 20;
    ui->PORTtitle->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 10 - (double)HEIGHT / 20;
    ui->PORTinput->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));

    ui->serverStatus->setStyleSheet(ACCENT_COLOR);
    ui->clientStatus->setStyleSheet(ACCENT_COLOR);
    ui->restartBtn->setStyleSheet(ACCENT_COLOR);
    ui->reconnectBtn->setStyleSheet(ACCENT_COLOR);
        X = (double)WIDTH * 5 / 6 - (double)HEIGHT / 5,
        Y = (double)HEIGHT / 20 * 3 - (double)HEIGHT / 20;
    ui->serverStatus->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 5 - (double)HEIGHT / 20;
    ui->restartBtn->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 8 - (double)HEIGHT / 20;
    ui->clientStatus->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 10 - (double)HEIGHT / 20;
    ui->reconnectBtn->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));

    ui->switchBtn->setStyleSheet(ACCENT_COLOR);
    ui->usrnameInput->setStyleSheet(ACCENT_COLOR);
    ui->IDtitle->setStyleSheet(ACCENT_COLOR);
        X = (double)WIDTH * 5 / 7 - (double)HEIGHT / 5,
        Y = (double)HEIGHT / 20 * 14 - (double)HEIGHT / 20;
    ui->switchBtn->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        W = (double)HEIGHT / 2;
        X = (double)WIDTH * 2.2 / 6 - (double)W / 2;
    ui->usrnameInput->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
    ui->usrnameInput->setText(judge->usrnameOL);
        W = (double)HEIGHT / 15;
        X = X - W;
    ui->IDtitle->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
    ui->switchBtn->setDisabled(true);
    ui->usrnameInput->setDisabled(true);

    QRegularExpression regx("^[0-9a-zA-Z_]{1,}$");
    QValidator *validator = new QRegularExpressionValidator(regx, ui->usrnameInput);
    ui->usrnameInput->setValidator(validator);
    ui->usrnameInput->setMaxLength(18);

    connect(ui->gamemodeCB, &QComboBox::activated, this, &SettingDialog::getRunMode);
    connect(ui->chessbdCB, &QComboBox::activated, this, &SettingDialog::getChessBd);
    connect(judge->socket->base(), &QTcpSocket::connected, this, [&](){ui->clientStatus->setText("Connected");});
    connect(judge, &Judge::LEAVE_OP, this, [&](){ui->clientStatus->setText("Disconnected");});
    connect(judge->server, &QTcpServer::newConnection, this, [&](){ui->clientStatus->setText("Connected");});
}

SettingDialog::~SettingDialog()
{
    delete this;
}

void SettingDialog::autoFill()
{
    if(judge->runMode == 2 || judge->runMode == 3) return;
    ui->gamemodeCB->setCurrentIndex(judge->runMode);
    int sizeNum = 0;
    switch(judge->CHESSBOARD_SIZE)
    {
    case 9: sizeNum = 0; break;
    case 11: sizeNum = 1; break;
    case 13: sizeNum = 2; break;
    case 28: sizeNum = 3; break;
    }
    ui->chessbdCB->setCurrentIndex(sizeNum);
}

void SettingDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    // painter.setPen(QPen(QColor(0xf9f8ef), Strength, Qt::SolidLine));
    painter.setBrush(QColor(BG_COLOR));
    painter.drawRect(0, 0, WIDTH, HEIGHT);
}

void SettingDialog::getRunMode(int mode)
{
    judge->runMode = mode;
}
void SettingDialog::getChessBd(int size)
{
    int realSz;
    switch(size)
    {
        case 0: realSz = 9; break;
        case 1: realSz = 11; break;
        case 2: realSz = 13; break;
        case 3: realSz = 28; break;
        default: realSz = 9;
    }
    judge->CHESSBOARD_SIZE = realSz;
}

// 按钮行为
void SettingDialog::on_closeBtn_clicked()
{
    judge->IP = ui->IPinput->text();
    judge->PORT = ui->PORTinput->text().toInt();
    judge->usrnameOL = ui->usrnameInput->text();
    getChessBd(ui->chessbdCB->currentIndex());
    this->close();
}
void SettingDialog::on_restartBtn_clicked()
{
    judge->PORT = ui->PORTinput->text().toInt();
    judge->usrnameOL = ui->usrnameInput->text();
    foreach (QHostAddress address, QHostInfo::fromName(QHostInfo::localHostName()).addresses()) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            judge->IP = address.toString();
        }
    }

    ui->serverStatus->setText("Server: Running");
    ui->IPinput->setText(judge->IP);
    ui->IPtitle->setText("Server IP");
    ui->PORTtitle->setText("Server Port");
    ui->gamemodeCB->setCurrentIndex(1);
    ui->chessbdCB->setCurrentIndex(0);
    ui->reconnectBtn->setDisabled(true);
    ui->IPinput->setDisabled(true);
    ui->gamemodeCB->setDisabled(true);
    ui->chessbdCB->setDisabled(true);
    ui->switchBtn->setDisabled(false);
    ui->usrnameInput->setDisabled(false);

    judge->clearLink();
    judge->server->listen(QHostAddress::Any, judge->PORT);

    emit goOL();
    judge->runMode = 2;
}
void SettingDialog::on_reconnectBtn_clicked()
{
    judge->IP = ui->IPinput->text();
    judge->PORT = ui->PORTinput->text().toInt();
    judge->usrnameOL = ui->usrnameInput->text();

    ui->gamemodeCB->setCurrentIndex(1);
    ui->chessbdCB->setCurrentIndex(0);
    ui->restartBtn->setDisabled(true);
    ui->gamemodeCB->setDisabled(true);
    ui->chessbdCB->setDisabled(true);
    ui->switchBtn->setDisabled(false);
    ui->usrnameInput->setDisabled(false);

    judge->clearLink();
    judge->socket->hello(judge->IP, judge->PORT);
    if(!judge->socket->base()->waitForConnected(5000))
    {
        ui->clientStatus->setText("Connect Failed");
        judge->socketConnected = false;
    }
    else
    {
        judge->socket->send(NetworkData(OPCODE::CHAT_OP, "", ""));
        judge->socketConnected = true;
    }

    emit goOL();
    judge->runMode = 3;
}
void SettingDialog::on_switchBtn_clicked()
{
    // 发送 LEAVE_OP 信号并断开连接
    ui->serverStatus->setText("Server: Paused");
    ui->IPtitle->setText("Remote IP");
    ui->PORTtitle->setText("Remote Port");
    ui->clientStatus->setText("Not Connected");
    ui->gamemodeCB->setCurrentIndex(0);
    ui->chessbdCB->setCurrentIndex(0);
    ui->reconnectBtn->setDisabled(false);
    ui->restartBtn->setDisabled(false);
    ui->IPinput->setDisabled(false);
    ui->gamemodeCB->setDisabled(false);
    ui->chessbdCB->setDisabled(false);
    ui->switchBtn->setDisabled(true);
    ui->usrnameInput->setDisabled(true);

    judge->clearLink();

    emit goOFFL();
    judge->runMode = 0;
}
