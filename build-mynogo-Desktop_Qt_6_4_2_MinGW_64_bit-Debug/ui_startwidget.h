/********************************************************************************
** Form generated from reading UI file 'startwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTWIDGET_H
#define UI_STARTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StartWidget
{
public:
    QPushButton *startAsWhite;
    QPushButton *startAsBlack;

    void setupUi(QWidget *StartWidget)
    {
        if (StartWidget->objectName().isEmpty())
            StartWidget->setObjectName("StartWidget");
        StartWidget->resize(1068, 768);
        startAsWhite = new QPushButton(StartWidget);
        startAsWhite->setObjectName("startAsWhite");
        startAsWhite->setGeometry(QRect(434, 484, 200, 50));
        QFont font;
        font.setFamilies({QString::fromUtf8("Arial Rounded MT Bold")});
        font.setPointSize(20);
        font.setBold(true);
        startAsWhite->setFont(font);
        startAsWhite->setFlat(true);
        startAsBlack = new QPushButton(StartWidget);
        startAsBlack->setObjectName("startAsBlack");
        startAsBlack->setGeometry(QRect(434, 584, 200, 50));
        startAsBlack->setFont(font);
        startAsBlack->setFlat(true);

        retranslateUi(StartWidget);

        QMetaObject::connectSlotsByName(StartWidget);
    } // setupUi

    void retranslateUi(QWidget *StartWidget)
    {
        StartWidget->setWindowTitle(QCoreApplication::translate("StartWidget", "Form", nullptr));
        startAsWhite->setText(QCoreApplication::translate("StartWidget", "Play as White", nullptr));
        startAsBlack->setText(QCoreApplication::translate("StartWidget", "Play as Black", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StartWidget: public Ui_StartWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTWIDGET_H
