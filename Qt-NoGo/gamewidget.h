#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "judge.h"
#include <QPushButton>
#include <QPainter>
#include <QWidget>
#include <QMouseEvent>

namespace Ui { class GameWidget; }

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

    Judge *judge;

signals:
    void restartSingal(int index);
public slots:
    void on_restartButton_clicked();


private:
    Ui::GameWidget *ui;
    void paintEvent(QPaintEvent *event) override;
    void drawChessboard(QPainter &painter);
    void drawBlack(QPainter &painter, double px, double py);
    void drawWhite(QPainter &painer, double px, double py);
    void drawDemo(QPainter &painter);

    // 监听鼠标坐标
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // GAMEWIDGET_H
