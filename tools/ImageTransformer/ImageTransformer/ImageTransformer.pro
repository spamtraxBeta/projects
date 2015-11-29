TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    components/dirIterator/diriterator.cpp \
    imageProviders/transformProvider/transformprovider.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    components/dirIterator/diriterator.h \
    imageProviders/transformProvider/transformprovider.h

