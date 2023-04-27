#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QObject>
#include <QPainter>
#include <QColor>
#include "judge.h"

namespace Ui { class OptionDialog; }

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    /*
     * OptionDialog(QString m, int type, QWidget *parent = nullptr)
     * type = 0:  yes/no 按钮
     * type = 1:  不含按钮
     * type = 2:  close 按钮
    */
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
