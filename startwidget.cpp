#include "startwidget.h"
#include "ui_startwidget.h"
#include "judge.h"

StartWidget::StartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWidget)
{
    ui->setupUi(this);
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // 设置按钮样式
    ui->startAsBlack->setStyleSheet(ACCENT_COLOR);
    ui->startAsWhite->setStyleSheet(ACCENT_COLOR);
    int H = (double)WINDOW_HEIGHT / 31 * 2,
        W = (double)WINDOW_HEIGHT / 31 * 8;
    int X = (double)WINDOW_WIDTH / 2 - (double)WINDOW_HEIGHT / 31 * 4,
        Y = (double)WINDOW_HEIGHT / 20 * 12.5 - (double)WINDOW_HEIGHT / 31;
    ui->startAsBlack->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));
        Y = (double)WINDOW_HEIGHT / 20 * 15 - (double)WINDOW_HEIGHT / 31;
    ui->startAsWhite->setGeometry(QRect(QPoint(X, Y), QSize(W, H)));

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
    painter.drawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
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
