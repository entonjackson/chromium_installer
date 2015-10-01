#ifndef APPHANDLER_H
#define APPHANDLER_H

#include "filedownloader.h"

#include <QObject>
#include <QUrl>

class AppHandler : public QObject
{
    Q_OBJECT

public:
    AppHandler();
    QUrl getLatestUrl();

signals:
    void finished();
    void updateProgress(float progress);

public:
    Q_INVOKABLE void downloadLatest();

protected slots:
    void downloadFinished();

protected:
    bool fileExist(const QUrl &url);
    void installApk(const QString& apk);

private:
    FileDownloader* m_downloader;
};

#endif // APPHANDLER_H
