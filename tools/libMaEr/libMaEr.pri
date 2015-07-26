
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/MaEr/urlencode/urlencode.hpp \
    $$PWD/MaEr/toggle/toggle.hpp \
    $$PWD/MaEr/trace/trace.hpp \
    $$PWD/MaEr/types/byte_t.hpp \
    $$PWD/MaEr/log/logHelper.hpp \
    $$PWD/MaEr/unusedvariable.hpp \
    $$PWD/MaEr/boostHelpers/asioTimeoutReader.hpp

SOURCES += \
    $$PWD/MaEr/urlencode/urlencode.cpp \
    $$PWD/MaEr/toggle/toggle.cpp \
    $$PWD/MaEr/types/byte_t.cpp \
    $$PWD/MaEr/log/logHelper.cpp \
    $$PWD/MaEr/boostHelpers/asioTimeoutReader.cpp

LIBS += \
    -lboost_system \
    -lboost_regex \
    -lboost_log \
    -lboost_thread \
    -lboost_filesystem \
    -lboost_log_setup \
    -lboost_random \
    -lpthread \
    -lrt


QMAKE_CXXFLAGS += -std=c++0x

DEFINES += BOOST_LOG_DYN_LINK

CONFIG(debug, debug|release){
# debug mode: http://www.qtcentre.org/threads/28049-qmake-Project-file-has-both-quot-debug-quot-and-quot-release-quot-conditions-true

}

CONFIG(release, debug|release){
# release mode: http://www.qtcentre.org/threads/28049-qmake-Project-file-has-both-quot-debug-quot-and-quot-release-quot-conditions-true

}
