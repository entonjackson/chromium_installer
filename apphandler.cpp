#include "apphandler.h"

#include <QDateTime>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QTcpSocket>
#include <QAndroidJniObject>
#include <QLatin1String>
#include <QGuiApplication>

QString ZIPNAME = "chrome-android.zip";

AppHandler::AppHandler()
{
    m_downloader = 0;
    m_downloading = false;
}

void AppHandler::downloadLatest()
{
    if (m_downloading)
        return;

    m_downloading = true;

    emit enableDownloadButton(false);
    qApp->processEvents();
    updateProgress(0.0);
    QUrl latestUrl = getLatestUrl();
    if (latestUrl.isEmpty()) {
        makeToast("Network Error :-(");
        return;
    }
    m_downloader = new FileDownloader(latestUrl);
    connect(m_downloader, SIGNAL(downloaded()), SLOT(downloadFinished()));
    connect(m_downloader, SIGNAL(updateProgress(float)), this, SIGNAL(updateProgress(float)));
}

QUrl AppHandler::getLatestUrl()
{
    QDateTime datetime1970(QDate(1970,1,1));
    unsigned int latestVersionAsNumber = 333843 + datetime1970.daysTo(QDateTime::currentDateTime());

    QUrl latestUrl("https://storage.googleapis.com/chromium-browser-continuous/Android/" + QString::number(latestVersionAsNumber) + "/" + ZIPNAME);

    short tries = 1;
    for (; !fileExist(latestUrl) && tries < 10; tries++) {
        latestUrl = QUrl("https://storage.googleapis.com/chromium-browser-continuous/Android/" + QString::number(latestVersionAsNumber-tries) + "/" + ZIPNAME);
    }

    if (tries == 20) {
        return QUrl();
    }

    return latestUrl;
}

void AppHandler::downloadFinished()
{
    updateProgress(0.8);
    QByteArray localDownloadedData = m_downloader->downloadedData();
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

    QFile file(path + QDir::separator() + ZIPNAME);
    file.open(QIODevice::WriteOnly);
    file.write(localDownloadedData);
    file.close();

    makeToast("Download finished, unzipping and installing. This can take some seconds. Please wait...");
    updateProgress(1.0);

    // unzip
    QAndroidJniObject zipFilePath = QAndroidJniObject::fromString(ZIPNAME);
    QAndroidJniObject extractPath = QAndroidJniObject::fromString("chromium-installer");
    QAndroidJniObject::callStaticMethod<void>("bz/jako/chromiumInstaller/Unzip", "extract", "(Ljava/lang/String;Ljava/lang/String;)V", zipFilePath.object<jstring>(), extractPath.object<jstring>());

    // install apk
    QAndroidJniObject mediaDir = QAndroidJniObject::callStaticObjectMethod("android/os/Environment", "getExternalStorageDirectory", "()Ljava/io/File;");
    QAndroidJniObject mediaPath = mediaDir.callObjectMethod( "getAbsolutePath", "()Ljava/lang/String;" );

    QString apk = mediaPath.toString() + "/" + extractPath.toString() + "/chrome-android/apks/ChromePublic.apk";
    installApk(apk);
    updateProgress(1.0);
    m_downloading = false;
    emit enableDownloadButton(true);
}

bool AppHandler::fileExist(const QUrl &url)
{
    QTcpSocket socket;
    socket.connectToHost(url.host(), 80);
    if (socket.waitForConnected()) {
        socket.write("HEAD " + url.path().toUtf8() + " HTTP/1.1\r\n"
                                                     "Host: " + url.host().toUtf8() + "\r\n"
                                                                                      "\r\n");
        if (socket.waitForReadyRead()) {
            QByteArray bytes = socket.readAll();
            if (bytes.contains("200 OK"))
                return true;
        }
    }
    return false;
}

void AppHandler::installApk(const QString& apk) {
    QAndroidJniObject message = QAndroidJniObject::fromString(apk);
    QAndroidJniObject::callStaticMethod<void>("bz/jako/chromiumInstaller/AppInstaller", "installAPK", "(Ljava/lang/String;)V", message.object<jstring>());
}

void AppHandler::makeToast(const QString& message) {
    QAndroidJniObject msg = QAndroidJniObject::fromString(message);
    QAndroidJniObject::callStaticMethod<void>("bz/jako/chromiumInstaller/MyToast", "toast", "(Ljava/lang/String;)V", msg.object<jstring>());
}
