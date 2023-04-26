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
    SettingDialog(Judge *j, QWidget *parent = nullptr);
    ~SettingDialog();

signals:
    void toConnect();

public slots:
    void on_closeBtn_clicked();
    void on_restartBtn_clicked();
    void on_reconnectBtn_clicked();
    void getChessBd(int size);
    void getRunMode(int mode);

private:
    void paintEvent(QPaintEvent *event) override;

    Ui::SettingDialog *ui;
    int WIDTH, HEIGHT;
    Judge *judge;
};

#endif // SETTINGDIALOG_H
