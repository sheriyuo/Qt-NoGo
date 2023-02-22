#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QPushButton>
#include <QPainter>
#include <QWidget>

namespace Ui { class StartWidget; }

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    StartWidget(QWidget *parent = nullptr);
    ~StartWidget();

    int curGameLayer;
signals:
    void startAsWhiteSingal(int layerIndex);
    void startAsBlackSingal(int layerIndex);
    void startAs(int player); // 0->white  1->black
public slots:
    void on_startAsWhite_clicked();
    void on_startAsBlack_clicked();

private:
    Ui::StartWidget *ui;
    void paintEvent(QPaintEvent *event) override;

};

#endif // STARTWIDGET_H
