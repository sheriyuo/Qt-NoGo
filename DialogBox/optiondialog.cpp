#include "DialogBox/optiondialog.h"
#include "ui_optiondialog.h"

/*
*   @file: optiondialog.h
*   @brief: 声明 OptionDialog 类，
*           在联机对局时弹出申请窗口等
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*
*   OptionDialog(QString m, int type, QWidget *parent = nullptr)
*   弹出一个含有提示词 m 的选项框，所含按钮如下
*   type = 0:  yes/no 按钮
*   type = 1:  不含按钮
*   type = 2:  close 按钮
*/
OptionDialog::OptionDialog(QString m, int type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
    setFixedSize(QSize(270, 170));

    ui->messBox->setText(m);
    ui->messBox->setStyleSheet(ACCENT_COLOR);
    ui->okBtn->setStyleSheet(ACCENT_COLOR);
    ui->noBtn->setStyleSheet(ACCENT_COLOR);
    ui->closeBtn->setStyleSheet(ACCENT_COLOR);
    if(type != 2)
    {
        ui->closeBtn->setGeometry(QRect(QPoint(0, 0), QSize(0, 0)));
    }

    if(type == 1 || type == 2)
    {
        ui->okBtn->setGeometry(QRect(QPoint(0, 0), QSize(0, 0)));
        ui->noBtn->setGeometry(QRect(QPoint(0, 0), QSize(0, 0)));
    }
    if(type == 1)
    {
        setFixedSize(QSize(270, 130));
    }

    connect(ui->okBtn, &QPushButton::clicked, this, [&](){this->close();emit OK();});
    connect(ui->noBtn, &QPushButton::clicked, this, [&](){this->close();emit NO();});
}

OptionDialog::~OptionDialog()
{
    delete this;
}

void OptionDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    // painter.setPen(QPen(QColor(0xf9f8ef), Strength, Qt::SolidLine));
    painter.setBrush(QColor(BG_COLOR));
    painter.drawRect(0, 0, frameGeometry().width(), frameGeometry().height());
}

void OptionDialog::setMessage(QString m)
{
    ui->messBox->setText(m);
}

void OptionDialog::on_closeBtn_clicked()
{
    this->close();
}
