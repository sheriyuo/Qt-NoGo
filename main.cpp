#include <QApplication>
#include <QFontDatabase>
#include <QStackedLayout>
#include "Widget/gamewidget.h"
#include "Widget/gamewidgetol.h"
#include "Widget/startwidget.h"
#include "Object/judge.h"
#include "DialogBox/optiondialog.h"

int main(int argc, char *argv[])
{
     QApplication a(argc, argv);

    // 建立主窗口
    QWidget *w = new QWidget;

    // 创建对象
    Judge *judge = new Judge;
    Bot *bot = new Bot(judge);
    GameWidget *gameWidget = new GameWidget(judge, bot);
    GameWidgetOL *gameWidgetOL = new GameWidgetOL(judge, bot);
    StartWidget *startWidget = new StartWidget(judge);
    QStackedLayout *stackLayout = new QStackedLayout(w);
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // 信号槽
    QObject::connect(startWidget, &StartWidget::switchLayer, stackLayout, &QStackedLayout::setCurrentIndex);
    QObject::connect(startWidget, &StartWidget::startAs, judge, &Judge::setPlayerRole);
    QObject::connect(startWidget, &StartWidget::startAs, bot, &Bot::makeFirstMove);
    QObject::connect(startWidget, &StartWidget::startAs, gameWidget, &GameWidget::startTimer);
    QObject::connect(startWidget, &StartWidget::startAs, gameWidgetOL, &GameWidget::startTimer);
    QObject::connect(startWidget->settingDialog, &SettingDialog::goOL, startWidget, [&](){startWidget->curGameLayer = 2;});
    QObject::connect(startWidget->settingDialog, &SettingDialog::goOFFL, startWidget, [&](){startWidget->curGameLayer = 1;});
    QObject::connect(gameWidget, &GameWidget::restartSingal, stackLayout, &QStackedLayout::setCurrentIndex);
    QObject::connect(gameWidget, &GameWidget::resignSingal, stackLayout, &QStackedLayout::setCurrentIndex);
    QObject::connect(gameWidgetOL, &GameWidget::restartSingal, stackLayout, &QStackedLayout::setCurrentIndex);
    QObject::connect(gameWidgetOL, &GameWidget::resignSingal, stackLayout, &QStackedLayout::setCurrentIndex);
    QObject::connect(gameWidget, &GameWidget::turnForBot, bot, &Bot::makeAMove);
    QObject::connect(judge, &Judge::modifiedCB, gameWidget, &GameWidget::updateCB);
    QObject::connect(judge, &Judge::modifiedCB, gameWidgetOL, &GameWidget::updateCB);
    QObject::connect(judge, &Judge::MOVE_OP, gameWidgetOL, &GameWidget::startTimer);

    // 建立栈式布局
    stackLayout->addWidget(startWidget);
    stackLayout->addWidget(gameWidget);
    stackLayout->addWidget(gameWidgetOL);
    stackLayout->setCurrentIndex(0);
    mainLayout->addLayout(stackLayout);

    // 展示主窗口
    w->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    w->setLayout(mainLayout);
    w->setWindowTitle(QString("NoGo"));
    w->setWindowIcon(QIcon(":/img/icon.ico"));
    w->show();

    return a.exec();
}
