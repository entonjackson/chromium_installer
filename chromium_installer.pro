QT += quick androidextras network

# Wichtig für Java Code
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

SOURCES += main.cpp \
    filedownloader.cpp \
    apphandler.cpp \
    androidutils.cpp

RESOURCES += qml.qrc

HEADERS += \
    filedownloader.h \
    apphandler.h \
    androidutils.h

DISTFILES += \
    android-sources/src/bz/jako/chromiumInstaller/MyToast.java \
    android-sources/src/bz/jako/chromiumInstaller/Unzip.java \
    android-sources/src/bz/jako/chromiumInstaller/AppInstaller.java \
    android-sources/AndroidManifest.xml \
    android-sources/res/values/libs.xml \

lupdate_only{
    SOURCES = main.qml
}

TRANSLATIONS    +=  localization/localization_de.ts
