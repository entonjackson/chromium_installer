#include "filedownloader.h"



FileDownloader::FileDownloader(QUrl url, QObject *parent) : QObject(parent)
{
    m_bytes = 0;
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)), this, SLOT (fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(url);
    QNetworkReply* reply = m_WebCtrl.get(request);

    connect(reply, SIGNAL(bytesWritten(qint64)), SLOT(handleBytes(qint64)));
}

void FileDownloader::fileDownloaded(QNetworkReply* reply) {
    m_DownloadedData = reply->readAll();
    reply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const {
    return m_DownloadedData;
}

// TODO: hier kommt er nicht rein!
void FileDownloader::handleBytes(qint64 bytes) {
    emit updateProgress(0.8);
    m_bytes += bytes;
    if (m_bytes % 2 == 0) {
        emit updateProgress(m_bytes/54000000);
    }
}
