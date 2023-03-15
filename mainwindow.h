#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include "gamewidget.h"
#include "startwidget.h"
#include "judge.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void restart();

    StartWidget *startWidget;
    GameWidget *gameWidget;
    QStackedLayout *stackLayout;
};

#endif // MAINWINDOW_H
