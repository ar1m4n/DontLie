#-------------------------------------------------
#
# Project created by QtCreator 2018-02-15T14:01:27
#
#-------------------------------------------------

QT += core gui webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DontLie
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        DbManager.cpp \
    Advertising.cpp \
    ImageHash.cpp

HEADERS += \
        mainwindow.h \
        DbManager.h

FORMS += \
        mainwindow.ui \

ODB_FILES += \
        allmodels.hxx \

ODB_FLAGS = --database sqlite --profile qt --generate-schema --generate-query --generate-session
ODB_FLAGS += -I/home/default/Qt/5.8/gcc_64/include/ -I/home/default/Qt/5.8/gcc_64/include/QtCore
ODB_FLAGS += --std c++11
ODB_FLAGS += -x -fPIC

QMAKE_CXXFLAGS_WARN_ON = $$QMAKE_CXXFLAGS_WARN_ON -Wno-unknown-pragmas

LIBS += -lodb-sqlite
LIBS += -lodb-qt
LIBS += -lodb

for(dir, ODB_FILES) {
    ODB_PWD_FILES += $$PWD/$${dir}
}

odb.name = odb ${QMAKE_FILE_IN}
odb.input = ODB_PWD_FILES
odb.output = ${QMAKE_FILE_BASE}-odb.cxx
odb.commands = odb $$ODB_FLAGS ${QMAKE_FILE_IN}
odb.depends = $$ODB_PWD_FILES
odb.variable_out = SOURCES
odb.clean = ${QMAKE_FILE_BASE}-odb.cxx ${QMAKE_FILE_BASE}-odb.hxx ${QMAKE_FILE_BASE}-odb.ixx ${QMAKE_FILE_BASE}.sql
QMAKE_EXTRA_COMPILERS += odb

odbh.name = odb ${QMAKE_FILE_IN}
odbh.input = ODB_PWD_FILES
odbh.output = ${QMAKE_FILE_BASE}-odb.hxx
odbh.commands = @true
odbh.CONFIG = no_link
odbh.depends = ${QMAKE_FILE_BASE}-odb.cxx
QMAKE_EXTRA_COMPILERS += odbh
