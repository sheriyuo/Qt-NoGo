#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(Judge *j, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog),
    judge(j)
{
    ui->setupUi(this);
    WIDTH = WINDOW_WIDTH * 3 / 5, HEIGHT = WINDOW_HEIGHT / 2;
    setFixedSize(WIDTH, HEIGHT);
    // setWindowIcon(QIcon(":/img/icon.png"));

    ui->closeBtn->setStyleSheet(ACCENT_COLOR);
    int H = (double)HEIGHT / 10,
        W = (double)HEIGHT / 5;
    int X = (double)WIDTH / 2 - (double)HEIGHT / 10,
        Y = (double)HEIGHT / 20 * 17 - (double)HEIGHT / 20;
    ui->closeBtn->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));

    // ui->gamemodeCB->addItem("");

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
        Y = (double)HEIGHT / 20 * 10 - (double)HEIGHT / 20;
    ui->chessbd->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 12 - (double)HEIGHT / 20;
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
        Y = (double)HEIGHT / 20 * 10 - (double)HEIGHT / 20;
    ui->PORTtitle->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 12 - (double)HEIGHT / 20;
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
        Y = (double)HEIGHT / 20 * 10 - (double)HEIGHT / 20;
    ui->clientStatus->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 12 - (double)HEIGHT / 20;
    ui->reconnectBtn->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));

    connect(ui->gamemodeCB, &QComboBox::activated, this, &SettingDialog::getRunMode);
    connect(ui->chessbdCB, &QComboBox::activated, this, &SettingDialog::getChessBd);
    connect(judge->socket->base(), &QTcpSocket::connected, this, [&](){this->ui->clientStatus->setText("Connected");});
}

SettingDialog::~SettingDialog()
{
    delete this;
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
    // qDebug() << judge->IP << ' ' << judge->PORT << '\n';
    this->close();
}
void SettingDialog::on_restartBtn_clicked()
{
    judge->IP = ui->IPinput->text();
    judge->PORT = ui->PORTinput->text().toInt();

    this->ui->serverStatus->setText("Server: Running");
    this->ui->PORTtitle->setText("Server Port");
    this->ui->gamemodeCB->setCurrentIndex(1);
    this->ui->chessbdCB->setCurrentIndex(0);
    this->ui->reconnectBtn->setDisabled(true);
    this->ui->IPinput->setDisabled(true);
    this->ui->gamemodeCB->setDisabled(true);
    this->ui->chessbdCB->setDisabled(true);

    disconnect(judge->server, &NetworkServer::receive, judge, &Judge::recData);
    delete judge->server;
    judge->server = new NetworkServer(judge);
    judge->server->listen(QHostAddress::Any, judge->PORT);
    connect(judge->server, &NetworkServer::receive, judge, &Judge::recData);

    emit goOnline();
}
void SettingDialog::on_reconnectBtn_clicked()
{
    judge->IP = ui->IPinput->text();
    judge->PORT = ui->PORTinput->text().toInt();

    this->ui->clientStatus->setText("Connecting");
    this->ui->gamemodeCB->setCurrentIndex(1);
    this->ui->chessbdCB->setCurrentIndex(0);
    this->ui->restartBtn->setDisabled(true);
    this->ui->gamemodeCB->setDisabled(true);
    this->ui->chessbdCB->setDisabled(true);

    // 关闭连接
    judge->socket->bye();
    // 新建连接
    judge->socket->hello(judge->IP, judge->PORT);
    if(!judge->socket->base()->waitForConnected(3000))
    {
        this->ui->clientStatus->setText("Connect Failed");
    }

    emit goOnline();
}
