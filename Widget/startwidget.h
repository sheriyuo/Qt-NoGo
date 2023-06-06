#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QPushButton>
#include <QPainter>
#include <QWidget>

#include "Object/judge.h"
#include "DialogBox/settingdialog.h"
#include "DialogBox/optiondialog.h"

namespace Ui { class StartWidget; }

/*
*   @file: startwidget.h
*   @brief: 声明 StartWidget 类，
*           实现欢迎界面
*   @author: sheriyuo, ce-amtic, duality314
*   @time: 2023/5/1
*/

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    StartWidget(Judge *j, QWidget *parent = nullptr);
    ~StartWidget();

    int curGameLayer; // 用于切换到 gameWidget 界面
    SettingDialog *settingDialog;
signals:
    void switchLayer(int layerIndex);
    void startAs(int player); // 0->white  1->black
private slots:
    // 按钮行为
    void on_startAsWhite_clicked_OFFL();
    void on_startAsBlack_clicked_OFFL();
    void on_startAsWhite_clicked_OL();
    void on_startAsBlack_clicked_OL();
    void on_settingsBtn_clicked();

    // 切换 online offline 槽函数
    void goOL();
    void goOFFL();

private:
    void paintEvent(QPaintEvent *event) override;
    void sendStartAsBlack(bool isReply = false); // 联机
    void sendStartAsWhite(bool isReply = false); // 联机
    void sendReject(); // 联机被拒

    Judge* judge;
    Ui::StartWidget *ui;

    // 联机对话框
    int oppoRole, inviterRole;
    OptionDialog* confirmD;
    OptionDialog* awaitD;
    OptionDialog* warnD;
};

#endif // STARTWIDGET_H
