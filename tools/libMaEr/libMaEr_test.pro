TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(libMaEr.pri)




LIBS += \
    -lboost_unit_test_framework


CONFIG(debug, debug|release){
# debug mode: http://www.qtcentre.org/threads/28049-qmake-Project-file-has-both-quot-debug-quot-and-quot-release-quot-conditions-true

}

CONFIG(release, debug|release){
# release mode: http://www.qtcentre.org/threads/28049-qmake-Project-file-has-both-quot-debug-quot-and-quot-release-quot-conditions-true

    # eliminate unused symbols: http://stackoverflow.com/questions/6687630/c-c-gcc-ld-remove-unused-symbols
    QMAKE_CXXFLAGS += -Os -fdata-sections -ffunction-sections -Wl,--gc-sections
}


include(../deployment.pri)
qtcAddDeployment()

SOURCES += \
    testcases/urlencode/test_urlencode.cpp \
    testcases/toggle/test_toggle.cpp



