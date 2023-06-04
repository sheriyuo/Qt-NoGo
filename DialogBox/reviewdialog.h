#ifndef REVIEWDIALOG_H
#define REVIEWDIALOG_H

#include <QDialog>
#include <QObject>
#include <QPainter>
#include <QColor>
#include <QRegularExpression>
#include <QValidator>
#include "Object/judge.h"
#include "DialogBox/messagebox.h"
/*
*   @file: optiondialog.h
*   @brief: 声明 ReviewDialog 类，
            实现复盘功能
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/26
*/

namespace Ui {
class ReviewDialog;
}

class ReviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReviewDialog(Judge *pjudge,QWidget *parent = nullptr);
    ~ReviewDialog();
    void set_review_data(char state,char data[],ItemVector v);
    void init();

signals:
    void trybutton();
    void quit_try();


public slots:
    void display();    
private slots:
    void on_start_clicked();

    void on_pause_clicked();

    void on_previous_clicked();

    void on_next_clicked();

    void on_tryButton_clicked();

    void on_quit_try_clicked();

    void on_input_entered();


private:
    void paintEvent(QPaintEvent *event) override;
    Ui::ReviewDialog *ui;

    int WIDTH=687, HEIGHT=204;  //dialog 大小
    MessageBox *messagebox;
    Judge *judge;
    char strState;
    char dataStr[28 * 28 * 3 + 5];
    ItemVector dataVec;
    int sum_steps;  //总共有多少步
    QTimer *tim;    //自动播放的计时器
    int now_step = 0;   //当前进行到哪一步
};

#endif // REVIEWDIALOG_H
