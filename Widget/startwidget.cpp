#include "Widget/startwidget.h"
#include "ui_startwidget.h"

/*
*   @file: startwidget.h
*   @brief: 声明 StartWidget 类，
*           实现欢迎界面
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*/

StartWidget::StartWidget(Judge *j, QWidget *parent) :
    QWidget(parent),
    judge(j),
    ui(new Ui::StartWidget)
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
        if(judge->playerRole != 0) // on GameWidget
            emit switchLayer(0);
        if(oppoRole == 1) {sendStartAsWhite(true); on_startAsWhite_clicked_OFFL();}
        else {sendStartAsBlack(true); on_startAsBlack_clicked_OFFL();}
        confirmD->close();
    });
    connect(confirmD, &OptionDialog::NO, this, [&](){
        sendReject();
        confirmD->close();
    });
    connect(judge, &Judge::READY_OP, this, [&](NetworkData d){
        awaitD->close();
        warnD->close();
        confirmD->close();
        confirmD->setMessage("New invitation from:\n"+judge->oppoOL+"\nConfirm playing "+(d.data2=="w"?"Black":"White")+"?");
        confirmD->show();
        oppoRole = d.data2 == "b" ? 1 : -1;
    });
    connect(judge, &Judge::READY_OP_ForInviter, this, [&](){
        if(inviterRole == 1) {on_startAsBlack_clicked_OFFL();}
        else {on_startAsWhite_clicked_OFFL();}
        awaitD->close();
    });
    connect(judge, &Judge::REJECT_OP, this, [&](){
        awaitD->close();
        confirmD->close();
        warnD->setMessage("You've been declined!");
        warnD->show();
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
    int H = (double)WINDOW_HEIGHT / 9,
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
void StartWidget::sendStartAsBlack(bool isReply)
{
    judge->send(NetworkData(OPCODE::READY_OP, judge->usrnameOL, isReply?"":"b"));
}
void StartWidget::sendStartAsWhite(bool isReply)
{
    judge->send(NetworkData(OPCODE::READY_OP, judge->usrnameOL, isReply?"":"w"));
}
void StartWidget::sendReject() {judge->send(NetworkData(OPCODE::REJECT_OP, judge->usrnameOL, ""));}

// 按钮行为
void StartWidget::on_settingsBtn_clicked()
{
    settingDialog->show();
    settingDialog->autoFill();
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
    if(!judge->isConnected())
    {
        confirmD->close();
        awaitD->close();
        warnD->setMessage("Not connected yet!");
        warnD->show();
    }
    else
    {
        confirmD->close();
        warnD->close();
        awaitD->show();

        inviterRole = 1;
        sendStartAsBlack();
    }
}
void StartWidget::on_startAsWhite_clicked_OL() // -1->white
{
    if(!judge->isConnected())
    {
        confirmD->close();
        awaitD->close();
        warnD->setMessage("Not connected yet!");
        warnD->show();
    }
    else
    {
        confirmD->close();
        warnD->close();
        awaitD->show();

        inviterRole = -1;
        sendStartAsWhite();
    }
}
