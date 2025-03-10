QT += core gui network serialport printsupport concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    general.cpp \
    infowidget.cpp \
    main.cpp \
    mainwindow.cpp \
    $$files(*.cpp) \
    $$files(*.c) \
    signals.cpp \
    timejump.cpp


HEADERS += \
        general.h \
        infowidget.h \
        mainwindow.h \
        $$files(*.h) \
        signals.h \
        timejump.h

FORMS += \
    General.ui \
    infowidget.ui \
    mainwindow.ui \
    signals.ui \
    timejump.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
res.qrc


FORMS += \
    mainwindow.ui \
    timejump.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
res.qrc
