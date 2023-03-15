#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    startWidget = new StartWidget;
    gameWidget = new GameWidget;
    stackLayout = new QStackedLayout;

    // 建立界面
    stackLayout->addWidget(startWidget);
    stackLayout->addWidget(gameWidget);
    stackLayout->setCurrentIndex(0);

    // 信号槽
    connect(startWidget, &StartWidget::startAsBlackSingal, stackLayout, &QStackedLayout::setCurrentIndex);
    connect(startWidget, &StartWidget::startAsWhiteSingal, stackLayout, &QStackedLayout::setCurrentIndex);
    connect(startWidget, &StartWidget::startAs, gameWidget->judge, &Judge::setPlayerRole);
    connect(startWidget, &StartWidget::startAs, gameWidget->bot, &Bot::makeFirstMove);
    connect(startWidget, &StartWidget::startAs, gameWidget, &GameWidget::startTimer);
    connect(gameWidget, &GameWidget::restartSingal, stackLayout, &QStackedLayout::setCurrentIndex);
    connect(gameWidget, &GameWidget::resignSingal, stackLayout, &QStackedLayout::setCurrentIndex);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(stackLayout);
    setLayout(mainLayout);
}

MainWindow::~MainWindow()
{
    // delete this;
}
