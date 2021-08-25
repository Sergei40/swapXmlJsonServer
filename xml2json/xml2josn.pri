INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

# Enable very detailed debug messages when compiling the debug version
CONFIG(debug, debug|release) {
    DEFINES += SUPERVERBOSE
}

HEADERS += $$PWD/include/xml2json.hpp \

