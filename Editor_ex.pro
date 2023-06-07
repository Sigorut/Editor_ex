QT       += core gui webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    edit_ex.cpp \
    filter.cpp \
    gen_28line.cpp \
    main.cpp \
    mainwindow.cpp \
    select_type_ex.cpp \
    table_gen_code.cpp

HEADERS += \
    edit_ex.h \
    filter.h \
    gen_28line.h \
    mainwindow.h \
    select_type_ex.h \
    table_gen_code.h

FORMS += \
    edit_ex.ui \
    filter.ui \
    gen_28line.ui \
    mainwindow.ui \
    select_type_ex.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    html/qrchtml.qrc \
    icons/qrcicons.qrc \
    js/qrcjs.qrc
