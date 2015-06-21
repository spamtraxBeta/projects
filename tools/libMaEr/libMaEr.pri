
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/MaEr/urlencode/urlencode.hpp \
    $$PWD/MaEr/toggle/toggle.hpp \
    $$PWD/MaEr/trace/trace.hpp \
    $$PWD/MaEr/types/byte_t.hpp \

SOURCES += \
    $$PWD/MaEr/urlencode/urlencode.cpp \
    $$PWD/MaEr/toggle/toggle.cpp \
    $$PWD/MaEr/types/byte_t.cpp \

LIBS += \
    -lboost_system \
    -lboost_regex


QMAKE_CXXFLAGS += -std=c++0x

CONFIG(debug, debug|release){
# debug mode: http://www.qtcentre.org/threads/28049-qmake-Project-file-has-both-quot-debug-quot-and-quot-release-quot-conditions-true

}

CONFIG(release, debug|release){
# release mode: http://www.qtcentre.org/threads/28049-qmake-Project-file-has-both-quot-debug-quot-and-quot-release-quot-conditions-true

}
