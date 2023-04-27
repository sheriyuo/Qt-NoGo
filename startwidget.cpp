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

    // 联机提示框   谁想出来的写联机逻辑，你摩洛哥炒饼，我重拾你的梦
    confirmD = new OptionDialog("New online invitation.\nDo you confirm?", 0, this);
    awaitD = new OptionDialog("Awaiting player\nconfirmation...", 1, this);
    warnD = new OptionDialog("Not connected yet!", 2, this);

    connect(settingDialog, &SettingDialog::goOL, this, &StartWidget::goOL);
    connect(settingDialog, &SettingDialog::goOFFL, this, &StartWidget::goOFFL);
    connect(ui->startAsBlack, &QPushButton::clicked, this, &StartWidget::on_startAsBlack_clicked_OFFL);
    connect(ui->startAsWhite, &QPushButton::clicked, this, &StartWidget::on_startAsWhite_clicked_OFFL);
    connect(confirmD, &OptionDialog::OK, this, [&](){
        if(oppoRole == 1) {sendStartAsWhite(1); on_startAsWhite_clicked_OFFL();}
        else {sendStartAsBlack(1); on_startAsBlack_clicked_OFFL();}
        confirmD->close();
    });
    connect(confirmD, &OptionDialog::NO, this, [&](){
        sendReject();
        confirmD->close();
    });
    connect(judge, &Judge::READY_OP, this, [&](int _oppoRole){confirmD->show();oppoRole = _oppoRole;});
    connect(judge, &Judge::READY_OP_ForInviter, this, [&](){
        if(inviterRole == 1) {on_startAsBlack_clicked_OFFL();}
        else {on_startAsWhite_clicked_OFFL();}
        awaitD->close();
    });
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

// 联机相关
void StartWidget::goOL()
{
    disconnect(ui->startAsBlack, &QPushButton::clicked, this, &StartWidget::on_startAsBlack_clicked_OFFL);
    disconnect(ui->startAsWhite, &QPushButton::clicked, this, &StartWidget::on_startAsWhite_clicked_OFFL);
    connect(ui->startAsBlack, &QPushButton::clicked, this, &StartWidget::on_startAsBlack_clicked_OL);
    connect(ui->startAsWhite, &QPushButton::clicked, this, &StartWidget::on_startAsWhite_clicked_OL);
}
void StartWidget::goOFFL()
{
    disconnect(ui->startAsBlack, &QPushButton::clicked, this, &StartWidget::on_startAsBlack_clicked_OL);
    disconnect(ui->startAsWhite, &QPushButton::clicked, this, &StartWidget::on_startAsWhite_clicked_OL);
    connect(ui->startAsBlack, &QPushButton::clicked, this, &StartWidget::on_startAsBlack_clicked_OFFL);
    connect(ui->startAsWhite, &QPushButton::clicked, this, &StartWidget::on_startAsWhite_clicked_OFFL);
}
void StartWidget::sendStartAsBlack(bool isFeedback)
{
    NetworkData d = NetworkData(OPCODE::READY_OP, "1", "");
    if(isFeedback) d = NetworkData(OPCODE::READY_OP, "", "");
    if(judge->runMode == 2)
    {
        judge->server->send(judge->lastClient, d);
    }
    else
    {
        judge->socket->send(d);
    }
}
void StartWidget::sendStartAsWhite(bool isFeedback)
{
    NetworkData d = NetworkData(OPCODE::READY_OP, "-1", "");
    if(isFeedback) d = NetworkData(OPCODE::READY_OP, "", "");
    if(judge->runMode == 2)
    {
        judge->server->send(judge->lastClient, d);
    }
    else
    {
        judge->socket->send(d);
    }
}
void StartWidget::sendReject()
{
    if(judge->runMode == 2)
    {
        judge->server->send(judge->lastClient, NetworkData(OPCODE::REJECT_OP, "", ""));
    }
    else
    {
        judge->socket->send(NetworkData(OPCODE::REJECT_OP, "", ""));
    }
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
void StartWidget::on_startAsBlack_clicked_OL() // 1->black
{
    if(judge->runMode == 2 && !judge->lastClient)
    {
        warnD->show();
    }
    else
    {
        awaitD->show();
        inviterRole = 1;
        sendStartAsBlack(0);
    }
}
void StartWidget::on_startAsWhite_clicked_OL() // -1->white
{
    if(judge->runMode == 2 && !judge->lastClient)
    {
        warnD->show();
    }
    else
    {
        sendStartAsWhite(0);
        inviterRole = -1;
        awaitD->show();
    }
}
