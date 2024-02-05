QT += serialport network

HEADERS += \
    $$PWD/include/aes/aesni-enc-ecb.h \
    $$PWD/include/aes/qaesencryption.h \
    $$PWD/include/cmricontrol.h \
    $$PWD/include/functiontest.h \
    $$PWD/include/lasercommunication.h \
    $$PWD/include/metercalibration.h \
    $$PWD/include/metercommunication.h \
    $$PWD/include/metererrortest.h \
    $$PWD/include/meterinfo.h \
    $$PWD/include/meterkeys.h \
    $$PWD/include/meterstartingcurrenttest.h \
    $$PWD/include/nicsynctest.h \
    $$PWD/include/powersource.h \
    $$PWD/include/pvfailtransition.h \
    $$PWD/include/pvstate.h \
    $$PWD/include/pvstatemachine.h \

INCLUDEPATH += "$$PWD/include"
LIBS += -L"$$PWD/lib/" -lcommunication
#LIBS += -L"$$PWD/release/" -lcommunication

