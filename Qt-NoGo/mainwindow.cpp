#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewidget.h"
#include "startwidget.h"
#include "judge.h"
#include "ui_startwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(PIC_WIDTH, PIC_HEIGHT);


    startWidget = new StartWidget;
    gameWidget = new GameWidget;
    stackLayout = new QStackedLayout;
    gameWidget->judge = new Judge;
    //startWidget = (StartWidget*)malloc(sizeof(StartWidget));
    //gameWidget = (GameWidget*)malloc(sizeof(GameWidget));
    //stackLayout = (QStackedLayout*)malloc(sizeof(QStackedLayout));
    //gameWidget->judge = (Judge*)malloc(sizeof(Judge));

    // 建立界面
    stackLayout->addWidget(startWidget);
    stackLayout->addWidget(gameWidget);
    stackLayout->setCurrentIndex(0);

    // 信号槽
    connect(startWidget, &StartWidget::startAsBlackSingal, stackLayout, &QStackedLayout::setCurrentIndex);
    connect(startWidget, &StartWidget::startAsWhiteSingal, stackLayout, &QStackedLayout::setCurrentIndex);
    connect(startWidget, &StartWidget::startAs, gameWidget->judge, &Judge::setPlayerRole);
    connect(gameWidget, &GameWidget::restartSingal, stackLayout, &QStackedLayout::setCurrentIndex);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(stackLayout);
    setLayout(mainLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
