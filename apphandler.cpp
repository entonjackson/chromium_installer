#include "apphandler.h"

#include "androidutils.h"

#include <QDateTime>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QTcpSocket>
#include <QAndroidJniObject>
#include <QLatin1String>
#include <QGuiApplication>
#include <QEventLoop>

QString ZIPNAME = "chrome-android.zip";
QString LAST_CHANGE = "https://www.googleapis.com/download/storage/v1/b/chromium-browser-continuous/o/Android%2FLAST_CHANGE?generation=1443865822830000&alt=media";

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
        AndroidUtils::makeToast("Network Error :-(");
        return;
    }
    m_downloader = new FileDownloader(latestUrl);
    connect(m_downloader, SIGNAL(downloaded()), SLOT(downloadFinished()));
    connect(m_downloader, SIGNAL(updateProgress(float)), this, SIGNAL(updateProgress(float)));
}

QUrl AppHandler::getLatestUrl()
{
    QNetworkAccessManager downloader;
    QNetworkReply* reply = downloader.get(QNetworkRequest(QUrl(LAST_CHANGE)));

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QString dataString(reply->readAll());

    reply->deleteLater();

    return QUrl("https://storage.googleapis.com/chromium-browser-continuous/Android/" + dataString + "/" + ZIPNAME);
}

void AppHandler::downloadFinished()
{
    updateProgress(0.8);
    QByteArray localDownloadedData = m_downloader->downloadedData();
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    AndroidUtils::makeToast(path);

    QFile file(path + QDir::separator() + ZIPNAME);
    file.open(QIODevice::WriteOnly);
    file.write(localDownloadedData);
    file.close();

    AndroidUtils::makeToast("Download finished... installing. This can take some seconds. Please wait...");
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
    m_downloading = false;
    emit enableDownloadButton(true);
    updateProgress(0.0);
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
