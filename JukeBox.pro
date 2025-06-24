QT += core gui
QT +=network
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/src

SOURCES += \
    src/backend/core/media.cpp \
    src/backend/core/movie.cpp \
    src/backend/core/playlist.cpp \
    src/backend/core/song.cpp \
    src/main.cpp \
    src/ui/mainwindow.cpp

HEADERS += \
    src/backend/core/media.h \
    src/backend/core/movie.h \
    src/backend/core/playlist.h \
    src/backend/core/song.h \
    src/backend/db/DBM.h \
    src/ui/mainwindow.h

FORMS += \
    ui/mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
