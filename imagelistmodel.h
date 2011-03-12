#ifndef IMAGELISTMODEL_H
#define IMAGELISTMODEL_H

#include <QAbstractListModel>
#include <QFileInfoList>
#include <QThreadPool>
#include <QImage>

#include "imageinfo.h"

class ImageListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ImageListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int imageCount() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    ImageInfo imageInfo(const QModelIndex &index) const;
    QString imagePath(const QModelIndex &index) const;

signals:
    void changed();

public slots:
    void addImageFileInfoList(const QFileInfoList &files);
    void clear();

private slots:
    void thumbnailLoaded(const QImage &image, int width, int height, int index);

private:
    QList<ImageInfo> m_imageInfoList;
    int m_imageInfoCount;

    QThreadPool m_threadPool;

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);
};

#endif // IMAGELISTMODEL_H
