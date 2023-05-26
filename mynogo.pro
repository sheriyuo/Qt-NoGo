QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    DialogBox/reviewdialog.h \
    Object/bot.h \
    Object/judge.h \
    Network/networkdata.h \
    Network/networkserver.h \
    Network/networksocket.h \
    DialogBox/messagebox.h \
    DialogBox/optiondialog.h \
    DialogBox/settingdialog.h \
    Object/logger.h \
    Object/switchcontrol.h \
    Widget/gamewidget.h \
    Widget/startwidget.h

SOURCES += \
    DialogBox/reviewdialog.cpp \
    Object/logger.cpp \
    Object/switchcontrol.cpp \
    main.cpp \
    Object/bot.cpp \
    Object/judge.cpp \
    Network/networkdata.cpp \
    Network/networkserver.cpp \
    Network/networksocket.cpp \
    DialogBox/messagebox.cpp \
    DialogBox/optiondialog.cpp \
    DialogBox/settingdialog.cpp \
    Widget/gamewidget.cpp \
    Widget/startwidget.cpp

FORMS += \
    DialogBox/optiondialog.ui \
    DialogBox/reviewdialog.ui \
    DialogBox/settingdialog.ui \
    Widget/gamewidget.ui \
    Widget/startwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc


