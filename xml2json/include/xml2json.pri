INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# Enable very detailed debug messages when compiling the debug version
CONFIG(debug, debug|release) {
    DEFINES += SUPERVERBOSE
}

HEADERS += $$PWD/xml2json.hpp \

