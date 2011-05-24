#ifndef IMAGELISTMODEL_H
#define IMAGELISTMODEL_H

#include <QAbstractListModel>
#include <QFileInfoList>
#include <QImage>

#include "imageinfo.h"
#include "imageloaderpool.h"

class ImageListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ImageListModel(QObject *parent = 0);

    inline int imageCount() const { return m_imageInfoList.count(); }
    inline int rowCount(const QModelIndex & = QModelIndex()) const { return m_imageInfoCount; }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    ImageInfo imageInfo(const QModelIndex &index) const;
    QString imagePath(const QModelIndex &index) const;

    bool hasCorruptedImages() const;
    inline bool allThumbnailsLoaded() const { return rowCount() == imageCount(); }

signals:
    void changed();

public slots:
    void addImage(const ImageInfo &imageInfo);
    void addImages(const QFileInfoList &files);
    void removeImage(const QModelIndex &index);
    void removeImages(const QModelIndex &startIndex, const QModelIndex &endIndex);
    void removeAllCorruptedImages();
    void clear();

private slots:
    void thumbnailLoaded(const QImage &image, int width, int height, int index);

private:
    static const QBrush ERROR_BRUSH;

    QList<ImageInfo> m_imageInfoList;
    int m_imageInfoCount;

    ImageLoaderPool m_imageLoaderPool;

    inline bool canFetchMore(const QModelIndex &) const { return m_imageInfoCount < m_imageInfoList.count(); }
    void fetchMore(const QModelIndex &parent);
};

#endif // IMAGELISTMODEL_H
