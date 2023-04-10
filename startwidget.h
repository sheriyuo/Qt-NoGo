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
    StartWidget(Judge *j, QWidget *parent = nullptr);
    ~StartWidget();

    int curGameLayer;
signals:
    void switchLayer(int layerIndex);
    void startAs(int player); // 0->white  1->black
public slots:
    void on_startAsWhite_clicked();
    void on_startAsBlack_clicked();
    void on_settingsBtn_clicked();

private:
    void paintEvent(QPaintEvent *event) override;

    Ui::StartWidget *ui;
    SettingDialog *settingDialog;
    Judge* judge;
};

#endif // STARTWIDGET_H
