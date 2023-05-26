QT       += core gui webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    edit_ex.cpp \
    editor.cpp \
    filter.cpp \
    main.cpp \
    mainwindow.cpp \
    select_type_ex.cpp

HEADERS += \
    edit_ex.h \
    editor.h \
    filter.h \
    mainwindow.h \
    select_type_ex.h

FORMS += \
    edit_ex.ui \
    editor.ui \
    filter.ui \
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
