#ifndef SCANFOLDERTHREAD_H
#define SCANFOLDERTHREAD_H

#include <QThread>
#include <QFileInfoList>
#include <QDir>
#include <QStringList>

class ScanFolderThread : public QThread
{
    Q_OBJECT

public:
    explicit ScanFolderThread(QObject *parent = 0);
    void setFolder(const QString &path, bool recursive = false);
    inline const QString& getFolder() const { return m_path; }
    void run();
    void abort();

signals:
    void folderScanned(const QString &folder, const QFileInfoList &files);

private:
    static QStringList FILE_FILTERS; // TODO should be const

    QString m_path;
    bool m_recursive;
    bool m_abort;

    void scanFolder(const QDir &dir);
    void scanFolderRecursively(const QString &path);
};

#endif // SCANFOLDERTHREAD_H
