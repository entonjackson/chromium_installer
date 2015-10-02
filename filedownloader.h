#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FileDownloader : public QObject
{
 Q_OBJECT
 public:
  explicit FileDownloader(QUrl url, QObject *parent = 0);
  QByteArray downloadedData() const;

 signals:
  void downloaded();
  void updateProgress(float progress);

 private slots:
  void fileDownloaded(QNetworkReply* pReply);
  void handleBytes(qint64 bytes, qint64 total);

 private:
  QNetworkAccessManager m_WebCtrl;
  QByteArray m_DownloadedData;
  QNetworkReply* m_Reply;
  qint64 m_bytes;
};

#endif // FILEDOWNLOADER_H
