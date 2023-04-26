#include "startwidget.h"
#include "ui_startwidget.h"

StartWidget::StartWidget(Judge *j, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWidget),
    judge(j)
{
    ui->setupUi(this);
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    // setWindowIcon(QIcon(":/img/icon.png"));

    // 设置按钮样式
    ui->startAsBlack->setStyleSheet(ACCENT_COLOR);
    ui->startAsWhite->setStyleSheet(ACCENT_COLOR);
    ui->settingsBtn->setStyleSheet(ACCENT_COLOR);
    int H = (double)WINDOW_HEIGHT / 31 * 2,
        W = (double)WINDOW_HEIGHT / 31 * 8;
    int X = (double)WINDOW_WIDTH / 2 - (double)WINDOW_HEIGHT / 31 * 4,
        Y = (double)WINDOW_HEIGHT / 20 * 11.5 - (double)WINDOW_HEIGHT / 31;
    ui->startAsBlack->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)WINDOW_HEIGHT / 20 * 14 - (double)WINDOW_HEIGHT / 31;
    ui->startAsWhite->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)WINDOW_HEIGHT / 20 * 16.5 - (double)WINDOW_HEIGHT / 31;
    ui->settingsBtn->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));

    // 游戏选项
    settingDialog = new SettingDialog(judge, this);
    curGameLayer = 1;

    connect(settingDialog, &SettingDialog::goOnline, this, &StartWidget::switchToOL);
    connect(ui->startAsBlack, &QPushButton::clicked, this, &StartWidget::on_startAsBlack_clicked_OFFL);
    connect(ui->startAsWhite, &QPushButton::clicked, this, &StartWidget::on_startAsWhite_clicked_OFFL);
}

StartWidget::~StartWidget()
{
    delete ui;
}

void StartWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    // painter.setPen(QPen(QColor(0xf9f8ef), Strength, Qt::SolidLine));
    painter.setBrush(QColor(BG_COLOR));
    painter.drawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    QPixmap logoImg;
    logoImg.load(":/img/logo.png");
    int H = (double)WINDOW_HEIGHT / 5,
        W = (double)H * logoImg.width() / logoImg.height();
    int X = (double)WINDOW_WIDTH / 2 - (double)W / 2,
        Y = (double)WINDOW_HEIGHT / 20 * 6 - (double)H / 2;
    painter.drawPixmap(X, Y, W, H, logoImg);
}

void StartWidget::switchToOL()
{
    disconnect(ui->startAsBlack, &QPushButton::clicked, this, &StartWidget::on_startAsBlack_clicked_OFFL);
    disconnect(ui->startAsWhite, &QPushButton::clicked, this, &StartWidget::on_startAsWhite_clicked_OFFL);
    connect(ui->startAsBlack, &QPushButton::clicked, this, &StartWidget::on_startAsBlack_clicked_OL);
    connect(ui->startAsWhite, &QPushButton::clicked, this, &StartWidget::on_startAsWhite_clicked_OL);
}

// 按钮行为
void StartWidget::on_settingsBtn_clicked()
{
    settingDialog->show();
}
void StartWidget::on_startAsBlack_clicked_OFFL()
{
    emit switchLayer(curGameLayer);
    emit startAs(1);
    this->close();
}
void StartWidget::on_startAsWhite_clicked_OFFL()
{
    emit switchLayer(curGameLayer);
    emit startAs(-1);
    this->close();
}
void StartWidget::on_startAsBlack_clicked_OL()
{
    MessageBox* mess = new MessageBox("还没写，别你吗急", 0, this);
    mess->show();
}
void StartWidget::on_startAsWhite_clicked_OL()
{
    MessageBox* mess = new MessageBox("还没写，别你吗急", 0, this);
    mess->show();
}
