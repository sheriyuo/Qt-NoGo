#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QObject>
#include <QPainter>
#include "judge.h"

namespace Ui { class SettingDialog; }

class SettingDialog : public QDialog
{
    Q_OBJECT
public:
    SettingDialog();
    ~SettingDialog();

signals:
    void rtChessBd(int size);
    void rtRunMode(int mode);

public slots:
    void on_closeBtn_clicked();
    void getChessBd(int size);
    void getRunMode(int mode);

private:
    void paintEvent(QPaintEvent *event) override;

    Ui::SettingDialog *ui;
    int WIDTH, HEIGHT;
};

#endif // SETTINGDIALOG_H
