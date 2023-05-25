#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QObject>
#include <QPainter>
#include <QColor>
#include "Object/judge.h"

namespace Ui { class OptionDialog; }

/*
*   @file: optiondialog.h
*   @brief: 声明 OptionDialog 类，
*           在联机对局时弹出申请窗口等
*
*           constructor:
*           > OptionDialog(QString m, int type, QWidget *parent = nullptr)
*             > 弹出一个含有提示词 m 的选项框，所含按钮如下
*             > type = 0:  yes/no 按钮
*             > type = 1:  不含按钮
*             > type = 2:  close 按钮
*
*           methods:
*           > void setMessage(QString m)
*
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*/

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    OptionDialog(QString m, int type, QWidget *parent = nullptr);
    ~OptionDialog();
    void setMessage(QString m);

signals:
    void OK();
    void NO();

public slots:
    void on_closeBtn_clicked();

private:
    void paintEvent(QPaintEvent *event) override;

    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
