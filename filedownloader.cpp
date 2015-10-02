#include "filedownloader.h"



FileDownloader::FileDownloader(QUrl url, QObject *parent) : QObject(parent)
{
    m_bytes = 0;
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)), this, SLOT (fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(url);
    m_Reply = m_WebCtrl.get(request);

    connect(m_Reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(handleBytes(qint64,qint64)));
}

void FileDownloader::fileDownloaded(QNetworkReply* reply) {
    m_DownloadedData = reply->readAll();
    reply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const {
    return m_DownloadedData;
}

void FileDownloader::handleBytes(qint64 bytes, qint64 total) {
    float progress = (double)bytes/(double)total;
    progress -= 0.2; // subtract 0.2 for unzip/install step
    if (progress > 0.0)
        emit updateProgress(progress);
}
