#include "apphandler.h"

#include <QDateTime>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QTcpSocket>
#include <QAndroidJniObject>
#include <QLatin1String>

AppHandler::AppHandler()
{
    m_downloader = 0;
}

void AppHandler::downloadLatest()
{
    QUrl latestUrl = getLatestUrl();
    m_downloader = new FileDownloader(latestUrl);
    connect(m_downloader, SIGNAL(downloaded()), SLOT(downloadFinished()));
    connect(m_downloader, SIGNAL(updateProgress(float)), this, SIGNAL(updateProgress(float)));
    // downloadFinished();
}

QUrl AppHandler::getLatestUrl()
{
    QDateTime datetime1970(QDate(1970,1,1));
    unsigned int latestVersionAsNumber = 333843 + datetime1970.daysTo(QDateTime::currentDateTime());

    QUrl latestUrl("https://storage.googleapis.com/chromium-browser-continuous/Android/" + QString::number(latestVersionAsNumber) + "/chrome-android.zip");

    short tries = 1;
    for (; !fileExist(latestUrl) && tries < 10; tries++) {
        latestUrl = QUrl("https://storage.googleapis.com/chromium-browser-continuous/Android/" + QString::number(latestVersionAsNumber-tries) + "/chrome-android.zip");
    }

    if (tries == 10) {
        return QUrl();
    }

    return latestUrl;
}

void AppHandler::downloadFinished()
{
    QByteArray localDownloadedData = m_downloader->downloadedData();
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

    QFile file(path + QDir::separator() + "bla.zip");
    file.open(QIODevice::WriteOnly);
    file.write(localDownloadedData);
    file.close();

    // show toast
    QAndroidJniObject message = QAndroidJniObject::fromString("Download finished, installing...");
    QAndroidJniObject::callStaticMethod<void>("bz/jako/chromiumInstaller/MyToast", "toast", "(Ljava/lang/String;)V", message.object<jstring>());

    // unzip
    QAndroidJniObject zipFilePath = QAndroidJniObject::fromString("bla.zip");
    QAndroidJniObject extractPath = QAndroidJniObject::fromString("bla_entpackt");
    QAndroidJniObject::callStaticMethod<void>("bz/jako/chromiumInstaller/Unzip", "extract", "(Ljava/lang/String;Ljava/lang/String;)V", zipFilePath.object<jstring>(), extractPath.object<jstring>());

    // message = QAndroidJniObject::fromString("Unzip finished!");
    // QAndroidJniObject::callStaticMethod<void>("bz/jako/chromiumInstaller/MyToast", "toast", "(Ljava/lang/String;)V", message.object<jstring>());

    // install apk
    QAndroidJniObject mediaDir = QAndroidJniObject::callStaticObjectMethod("android/os/Environment", "getExternalStorageDirectory", "()Ljava/io/File;");
    QAndroidJniObject mediaPath = mediaDir.callObjectMethod( "getAbsolutePath", "()Ljava/lang/String;" );

    QString apk = "/storage/sdcard0/bla_entpackt/chrome-android/apks/ChromePublic.apk";
    installApk(apk);
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
