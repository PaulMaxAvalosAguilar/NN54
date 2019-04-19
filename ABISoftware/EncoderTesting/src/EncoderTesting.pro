QT += widgets serialport


TARGET = EncoderTesting
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    testingconsole.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    testingconsole.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    testingconsole.ui

RESOURCES += \
    EncoderTesting.qrc

