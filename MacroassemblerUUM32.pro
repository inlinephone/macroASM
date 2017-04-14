QT += widgets
TEMPLATE = app
CONFIG += c++14 exceptions_off rtti_off stl_off qt

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    parser.cpp \
    macrokeywordparser.cpp \
    macrocalldeployer.cpp

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    console.h \
    parser.h \
    database.h \
    macrokeywordparser.h \
    macrocalldeployer.h

RESOURCES += \
    resources.qrc

