QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle
TEMPLATE = app
QT = core network

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        filedeletecontroller.cpp \
        fileuploadcontroller.cpp \
        global.cpp \
        main.cpp \
        requestmapper.cpp

OTHER_FILES += etc/* etc/docroot/* logs/*

#---------------------------------------------------------------------------------------
# The following lines include the sources of the QtWebAppLib library
#---------------------------------------------------------------------------------------
include(QtWebApp/QtWebApp/logging/logging.pri)
include(QtWebApp/QtWebApp/httpserver/httpserver.pri)

#---------------------------------------------------------------------------------------
# The following line include the sources of the jsonxx library
#---------------------------------------------------------------------------------------
include(jsonxx/jsonxx.pri)

#---------------------------------------------------------------------------------------
# The following line include the sources of the xml2json library
#---------------------------------------------------------------------------------------
include(xml2json/include/xml2json.pri)

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

HEADERS += \
    filedeletecontroller.h \
    fileuploadcontroller.h \
    global.h \
    requestmapper.h
