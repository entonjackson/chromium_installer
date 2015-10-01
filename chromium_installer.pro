QT += quick androidextras network

# Wichtig für Java Code
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

SOURCES += main.cpp \
    filedownloader.cpp \
    apphandler.cpp

RESOURCES += qml.qrc

HEADERS += \
    filedownloader.h \
    apphandler.h

DISTFILES += \
    android-sources/src/bz/jako/chromium_installer/MyToast.java \
    android-sources/src/bz/jako/chromium_installer/Unzip.java \
    android-sources/src/bz/jako/chromium_installer/AppInstaller.java \
    android-sources/AndroidManifest.xml \
    android-sources/gradle/wrapper/gradle-wrapper.jar \
    android-sources/gradlew \
    android-sources/res/values/libs.xml \
    android-sources/build.gradle \
    android-sources/gradle/wrapper/gradle-wrapper.properties \
    android-sources/gradlew.bat
