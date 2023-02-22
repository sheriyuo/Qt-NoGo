#include "startwidget.h"
#include "ui_startwidget.h"
#include "judge.h"

StartWidget::StartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWidget)
{
    ui->setupUi(this);
    setFixedSize(PIC_WIDTH, PIC_HEIGHT);

    ui->startAsBlack->setStyleSheet(STRESS_COLOR);
    ui->startAsWhite->setStyleSheet(STRESS_COLOR);

    curGameLayer = 1;
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
    painter.drawRect(0, 0, PIC_WIDTH, PIC_HEIGHT);
}

void StartWidget::on_startAsWhite_clicked()
{
    emit startAsWhiteSingal(curGameLayer);
    emit startAs(-1);
    this->close();
}
void StartWidget::on_startAsBlack_clicked()
{
    emit startAsBlackSingal(curGameLayer);
    emit startAs(1);
    this->close();
}
