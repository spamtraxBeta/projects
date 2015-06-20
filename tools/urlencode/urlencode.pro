TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../libMaEr/libMaEr.pri)

SOURCES += main.cpp


LIBS += \
    -lboost_program_options \
    -lboost_system \
    -lboost_regex

CCFLAG += -fdata-sections -ffunction-sections -Wl,--gc-sections



include(../deployment.pri)
qtcAddDeployment()

