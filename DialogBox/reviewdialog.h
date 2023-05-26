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
    explicit ReviewDialog(QWidget *parent = nullptr);
    ~ReviewDialog();

private:
    void paintEvent(QPaintEvent *event) override;
    Ui::ReviewDialog *ui;
    int WIDTH=687, HEIGHT=204;
};

#endif // REVIEWDIALOG_H
