#include "imagelistmodel.h"

#include "imageloader.h"

#include <QFileInfoList>
#include <QPixmap>
#include <QDateTime>
#include <QDir>

ImageListModel::ImageListModel(QObject *parent) :
    QAbstractListModel(parent), m_imageInfoCount(0)
{
}

int ImageListModel::rowCount(const QModelIndex &) const
{
    return m_imageInfoCount;
}

int ImageListModel::imageCount() const
{
    return m_imageInfoList.count();
}

QVariant ImageListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_imageInfoList.count())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_imageInfoList.at(index.row()).fileInfo().fileName();
    else if (role == Qt::DecorationRole)
        return m_imageInfoList.at(index.row()).icon();
    else if (role == Qt::ToolTipRole) {
        const ImageInfo info = m_imageInfoList.at(index.row());
        if (info.exists())
            return QString(tr("<html><b>%1</b><br/><br/>Dimensions: %2<br/>Size: %3<br/>Date created: %4</html>"))
                    .arg(QDir::toNativeSeparators(info.imagePath()))
                    .arg(info.dimensions())
                    .arg(info.size())
                    .arg(info.fileInfo().created().toString(Qt::DefaultLocaleShortDate));
        else
            return QString(tr("<html><b>%1<br/><br/><span style=\"color: red;\">IMAGE NOT FOUND</span></b></html>"))
                    .arg(QDir::toNativeSeparators(info.imagePath()));
    }

    return QVariant();
}

ImageInfo ImageListModel::imageInfo(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= m_imageInfoList.count())
        return ImageInfo();

    return m_imageInfoList.at(index.row());
}

QString ImageListModel::imagePath(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= m_imageInfoList.count())
        return QString();

    return m_imageInfoList.at(index.row()).imagePath();
}

void ImageListModel::addImage(const ImageInfo &imageInfo)
{
    beginInsertRows(QModelIndex(), m_imageInfoList.count(), m_imageInfoList.count() + 1);
    m_imageInfoList.append(imageInfo);
    endInsertRows();

    emit changed();
}

void ImageListModel::addImages(const QFileInfoList &files)
{
    if (files.count() == 0)
        return;

    beginInsertRows(QModelIndex(), m_imageInfoList.count(), m_imageInfoList.count() + files.count() - 1);
    foreach (const QFileInfo &file, files)
        m_imageInfoList.append(ImageInfo(file));
    endInsertRows();

    emit changed();
}

void ImageListModel::removeImage(const QModelIndex &index)
{
    if (!index.isValid() || index.row() >= m_imageInfoList.count())
        return;

    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_imageInfoList.removeAt(index.row());
    if (m_imageInfoCount > m_imageInfoList.count())
        m_imageInfoCount = m_imageInfoList.count();
    endRemoveRows();

    emit changed();
}

void ImageListModel::removeImages(const QModelIndex &startIndex, const QModelIndex &endIndex)
{
    if (!startIndex.isValid() || startIndex.row() >= m_imageInfoList.count() ||
            !endIndex.isValid() || endIndex.row() >= m_imageInfoList.count() ||
            startIndex.row() > endIndex.row())
        return;

    beginRemoveRows(QModelIndex(), startIndex.row(), endIndex.row());
    for (int i = startIndex.row(); i <= endIndex.row(); i++)
        m_imageInfoList.removeAt(i);
    if (m_imageInfoCount > m_imageInfoList.count())
        m_imageInfoCount = m_imageInfoList.count();
    endRemoveRows();

    emit changed();
}

void ImageListModel::clear()
{
    m_imageLoaderPool.clear();

    beginResetModel();
    m_imageInfoList.clear();
    m_imageInfoCount = 0;
    endResetModel();

    emit changed();
}

bool ImageListModel::canFetchMore(const QModelIndex &) const
{
    return m_imageInfoCount < m_imageInfoList.count();
}

void ImageListModel::fetchMore(const QModelIndex &)
{
    int remainder = m_imageInfoList.count() - m_imageInfoCount;
    int itemsToFetch = qMin(100, remainder);

    int beginIndex = m_imageInfoCount;
    int endIndex = beginIndex + itemsToFetch;
    if (endIndex >= m_imageInfoList.count())
        endIndex = m_imageInfoList.count() - 1;

    beginInsertRows(QModelIndex(), beginIndex, endIndex);
    m_imageInfoCount += itemsToFetch;
    endInsertRows();

    // start multithreaded image loading
    for (int i = beginIndex; i <= endIndex; i++) {
        const ImageInfo imageInfo = m_imageInfoList.at(i);
        if (imageInfo.exists()) {
            ImageLoader *imageLoader = new ImageLoader(imageInfo.imagePath(), i);
            imageLoader->setScaleSize();
            connect(imageLoader, SIGNAL(imageLoaded(QImage, int, int, int)), SLOT(thumbnailLoaded(QImage, int, int, int)));
            m_imageLoaderPool.start(imageLoader);
        }
    }

    emit changed();
    qDebug("ImageListModel::fetchMore(): from %d to %d", beginIndex, endIndex);
}

void ImageListModel::thumbnailLoaded(const QImage &image, int width, int height, int index)
{
    QModelIndex modelIndex = this->index(index);
    if (!modelIndex.isValid() || index >= m_imageInfoList.count())
        return;

    m_imageInfoList[index].setIcon(QPixmap::fromImage(image));
    m_imageInfoList[index].setDimensions(width, height);
    emit dataChanged(modelIndex, modelIndex);

    qDebug("ImageListModel::imageLoaded(%d)", index);
}
