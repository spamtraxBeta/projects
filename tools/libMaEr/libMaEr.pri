
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/MaEr/urlencode/urlencode.hpp \
    $$PWD/MaEr/toggle/toggle.hpp \
    $$PWD/MaEr/trace/trace.hpp

SOURCES += \
    $$PWD/MaEr/urlencode/urlencode.cpp \
    $$PWD/MaEr/toggle/toggle.cpp \

LIBS += \
    -lboost_system \
    -lboost_regex


CONFIG(debug, debug|release){
# debug mode: http://www.qtcentre.org/threads/28049-qmake-Project-file-has-both-quot-debug-quot-and-quot-release-quot-conditions-true

}

CONFIG(release, debug|release){
# release mode: http://www.qtcentre.org/threads/28049-qmake-Project-file-has-both-quot-debug-quot-and-quot-release-quot-conditions-true

}
