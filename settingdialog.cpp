#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog() :
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    WIDTH = WINDOW_WIDTH / 3, HEIGHT = WINDOW_HEIGHT / 2;
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
        X = (double)WIDTH / 2 - (double)HEIGHT / 5,
        Y = (double)HEIGHT / 20 * 3 - (double)HEIGHT / 20;
    ui->gamemode->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 5 - (double)HEIGHT / 20;
    ui->gamemodeCB->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 10 - (double)HEIGHT / 20;
    ui->chessbd->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)HEIGHT / 20 * 12 - (double)HEIGHT / 20;
    ui->chessbdCB->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));

    connect(ui->gamemodeCB, &QComboBox::activated, this, &SettingDialog::getRunMode);
    connect(ui->chessbdCB, &QComboBox::activated, this, &SettingDialog::getChessBd);
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

void SettingDialog::on_closeBtn_clicked()
{
    this->close();
}

void SettingDialog::getRunMode(int mode)
{
    emit rtRunMode(mode);
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
    emit rtChessBd(realSz);
}
