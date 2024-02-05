QT       += core gui qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -Werror=return-type
CONFIG += c++11

DEFINES += SIMULATOR_MODE
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    autotest/authenticateautotest.cpp \
    autotest/calibrationautotest.cpp \
    autotest/errorautotest.cpp \
    autotest/functionautotest.cpp \
    autotest/nicsyncautotest.cpp \
    calibrationerrortest.cpp \
    detectserialports.cpp \
    main.cpp \
    plccontroller.cpp \
    spmfulltest.cpp \
    widget.cpp \
    wizmeterview.cpp \
    wizplcliveview.cpp

HEADERS += \
    autotest/authenticateautotest.h \
    autotest/calibrationautotest.h \
    autotest/errorautotest.h \
    autotest/functionautotest.h \
    autotest/nicsyncautotest.h \
    calibrationerrortest.h \
    detectserialports.h \
    plccontroller.h \
    spmfulltest.h \
    widget.h \
    wizmeterview.h \
    wizplcliveview.h

FORMS += \
    widget.ui \
    wizmeterview.ui \
    wizplcliveview.ui

INCLUDEPATH += "lib/"

#include(communication/communication.pri)
include(lib_communication/libcommunication.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc
