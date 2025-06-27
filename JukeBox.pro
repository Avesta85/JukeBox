QT += core gui
QT +=network
QT += sql
QT += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

BOTAN_PATH = $$(BOTAN_ROOT)

isEmpty(BOTAN_PATH) {

    error("Environment variable 'BOTAN_ROOT' is not set. Please set it to your Botan installation path (e.g., C:/vcpkg/installed/x64-mingw-static).")
} else {

    message("Using Botan from environment variable: $$BOTAN_PATH")
}


INCLUDEPATH += $$BOTAN_PATH/include
LIBS += -L$$BOTAN_PATH/lib -lbotan-3

win32 {
    message("Configuring Botan for Windows platform...")
    DEFINES += BOTAN_TARGET_OS_IS_WINDOWS
}

SOURCES += \
    src/backend/core/SendEmail.cpp \
    src/backend/core/UserManager.cpp \
    src/backend/core/application.cpp \
    src/backend/core/application.cpp \
    src/backend/db/DBM.cpp \
    src/backend/core/person.cpp \
    src/backend/core/media.cpp \
    src/backend/core/movie.cpp \
    src/backend/core/playlist.cpp \
    src/backend/core/song.cpp \
    src/backend/core/user.cpp \
    src/backend/security/SecurityManager.cpp \
    src/main.cpp \
    src/ui/changepasswordwindow.cpp \
    src/ui/choicewindow.cpp \
    src/ui/emailverificationwindow.cpp \
    src/ui/frogotpasswordwindow.cpp \
    src/ui/loginwindow.cpp \
    src/ui/mainwindow.cpp \
    src/ui/receivesecurewordswindow.cpp \
    src/ui/signupwindow.cpp

HEADERS += \
    src/backend/core/SendEmail.h \
    src/backend/core/UserManager.h \
    src/backend/core/application.h \
    src/backend/core/application.h \
    src/backend/core/media.h \
    src/backend/core/movie.h \
    src/backend/core/person.h \
    src/backend/core/playlist.h \
    src/backend/core/song.h \
    src/backend/core/user.h \
    src/backend/db/DBM.h \
    src/ui/changepasswordwindow.h \
    src/ui/choicewindow.h \
    src/ui/emailverificationwindow.h \
    src/ui/frogotpasswordwindow.h \
    src/ui/loginwindow.h \
    src/ui/mainwindow.h \
    src/ui/receivesecurewordswindow.h \
    src/ui/signupwindow.h
    src/backend/security/SecurityManager.h \



FORMS += \
    ui/changepasswordwindow.ui \
    ui/choicewindow.ui \
    ui/emailverificationwindow.ui \
    ui/frogotpasswordwindow.ui \
    ui/loginwindow.ui \
    ui/mainwindow.ui \
    ui/receivesecurewordswindow.ui \
    ui/signupwindow.ui

QT += core
CONFIG += c++11

INCLUDEPATH += $$PWD/curl
LIBS += -L$$PWD/lib -lcurl


QT += network



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    res/images/157822-concert-fortnite_extended_set-electronic_dance_music-here_with_me-automotive_lighting-3840x2160.jpg

RESOURCES += \
    res/seoned-resorces.qrc
