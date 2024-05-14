QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dbmanager.cpp \
    debug.cpp \
    deliveryorderlistitem.cpp \
    driverloginwidget.cpp \
    driverpanel.cpp \
    futurefridgepanel.cpp \
    landingpage.cpp \
    loginwidget.cpp \
    main.cpp \
    notificationlistitem.cpp \
    settingswindow.cpp \
    productrowwidget.cpp \
    stafflistitem.cpp \
    stocksettingswidget.cpp \
    utils.cpp \

HEADERS += \
    dbmanager.h \
    debug.h \
    deliveryorderlistitem.h \
    driverloginwidget.h \
    driverpanel.h \
    environment.h \
    futurefridgepanel.h \
    landingpage.h \
    loginwidget.h \
    notificationlistitem.h \
    settingswindow.h \
    productrowwidget.h \
    stafflistitem.h \
    stocksettingswidget.h \
    utils.h

FORMS += \
    deliveryorderlistitem.ui \
    driverloginwidget.ui \
    driverpanel.ui \
    futurefridgepanel.ui \
    landingpage.ui \
    loginwidget.ui \
    notificationlistitem.ui \
    settingswindow.ui \
    productrowwidget.ui \
    stafflistitem.ui \
    stocksettingswidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# OpenSSL libraries
LIBS += -lssl -lcrypto

RESOURCES += \
    Resources.qrc
