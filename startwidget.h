#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QPushButton>
#include <QPainter>
#include <QWidget>
#include "judge.h"
#include "settingdialog.h"

namespace Ui { class StartWidget; }

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    StartWidget(QWidget *parent = nullptr);
    ~StartWidget();

    int curGameLayer;
    int runMode; // 0 : PVE    1 : PVP
    int gridSize;
signals:
    void startAsWhiteSingal(int layerIndex);
    void startAsBlackSingal(int layerIndex);
    void startAs(int player, int mode, int size); // 0->white  1->black
public slots:
    void on_startAsWhite_clicked();
    void on_startAsBlack_clicked();
    void on_settingsBtn_clicked();
    void setChessBd(int size);
    void setRunMode(int mode);

private:
    void paintEvent(QPaintEvent *event) override;

    Ui::StartWidget *ui;
    SettingDialog *settingDialog;
};

#endif // STARTWIDGET_H
