#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QObject>
#include <QPainter>
#include <QColor>
#include <QRegularExpression>
#include <QValidator>
#include "Object/judge.h"
#include "DialogBox/messagebox.h"

namespace Ui { class SettingDialog; }

/*
*   @file: settingdialog.h
*   @brief: 声明 SettingDialog 类，
*           实现图形化更改游戏选项
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*/

class SettingDialog : public QDialog
{
    Q_OBJECT
public:
    SettingDialog(Judge *j, QWidget *parent = nullptr);
    ~SettingDialog();

    void autoFill(); // 自动读取 judge 中的 runMode/CHESSBOARD_SIZE 等数据

signals:
    void goOL(); // 切换联机模式
    void goOFFL(); // 切换单机模式

public slots:
    void on_closeBtn_clicked(); // 关闭 按钮
    void on_restartBtn_clicked(); // 启动服务器 按钮
    void on_reconnectBtn_clicked(); // 连接服务器 按钮
    void on_switchBtn_clicked(); // 重新切换回单机模式 按钮
    void getChessBd(int size); // 获取棋盘大小 槽函数
    void getRunMode(int mode); // 获取单机游戏模式 PVP/PVE

private:
    void paintEvent(QPaintEvent *event) override;
    void closeEvent(QCloseEvent* event) override;

    Ui::SettingDialog *ui;
    int WIDTH, HEIGHT;
    Judge *judge;
};

#endif // SETTINGDIALOG_H
