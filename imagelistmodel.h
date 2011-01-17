#ifndef IMAGELISTMODEL_H
#define IMAGELISTMODEL_H

#include <QAbstractListModel>
#include <QFileInfoList>
#include <QImage>
#include <QIcon>

template <typename T> class QFutureWatcher;

class ImageListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ImageListModel(QObject *parent = 0);
    ~ImageListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QString filePath(const QModelIndex &index) const;

signals:
    void iconsLoaded();

public slots:
    void addImageFileInfoList(const QFileInfoList &files);
    void clear();

private slots:
    void thumbnailWatcher_resultReadyAt(int index);
    void thumbnailWatcher_finished();

protected:
    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

private:
    static QIcon DEFAULT_ICON;

    QFileInfoList m_fileList;
    QList<QIcon> m_iconList;
    int m_fileCount;

    QFutureWatcher<QImage> *m_thumbnailWatcher;
    static QImage createThumbnail(const QFileInfo &fileInfo);
    volatile int m_thumbnailIndex;

};

#endif // IMAGELISTMODEL_H
