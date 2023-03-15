#include <QApplication>
#include <QFontDatabase>
#include <QStackedLayout>
#include "gamewidget.h"
#include "startwidget.h"
#include "judge.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // MainWindow w;
    // w.show();

    // 建立界面
    QWidget *w = new QWidget;
    StartWidget *startWidget = new StartWidget;
    GameWidget *gameWidget = new GameWidget;
    QStackedLayout *stackLayout = new QStackedLayout(w);
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // 信号槽
    QObject::connect(startWidget, &StartWidget::startAsBlackSingal, stackLayout, &QStackedLayout::setCurrentIndex);
    QObject::connect(startWidget, &StartWidget::startAsWhiteSingal, stackLayout, &QStackedLayout::setCurrentIndex);
    QObject::connect(startWidget, &StartWidget::startAs, gameWidget->judge, &Judge::setPlayerRole);
    QObject::connect(startWidget, &StartWidget::startAs, gameWidget->bot, &Bot::makeFirstMove);
    QObject::connect(startWidget, &StartWidget::startAs, gameWidget, &GameWidget::startTimer);
    QObject::connect(gameWidget, &GameWidget::restartSingal, stackLayout, &QStackedLayout::setCurrentIndex);
    QObject::connect(gameWidget, &GameWidget::resignSingal, stackLayout, &QStackedLayout::setCurrentIndex);

    // 建立堆栈结构
    stackLayout->addWidget(startWidget);
    stackLayout->addWidget(gameWidget);
    stackLayout->setCurrentIndex(0);
    mainLayout->addLayout(stackLayout);

    w->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    w->setLayout(mainLayout);
    w->setWindowTitle(QString("NoGo"));
    w->setWindowIcon(QIcon(":/img/icon.ico"));
    w->show();

    return a.exec();
}
