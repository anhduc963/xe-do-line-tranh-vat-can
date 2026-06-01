QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# # Ten ung dung
TARGET = RobotControl
TEMPLATE = app

# # Cac tep nguon va tieu de
SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# # Mac dinh cho viec build
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
