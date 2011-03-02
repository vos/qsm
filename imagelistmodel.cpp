#include "imagelistmodel.h"

#include "imageloader.h"

#include <QFileInfoList>
#include <QPixmap>
#include <QDateTime>

ImageListModel::ImageListModel(QObject *parent) :
    QAbstractListModel(parent), m_fileCount(0)
{
}

int ImageListModel::rowCount(const QModelIndex &) const
{
    return m_fileCount;
}

int ImageListModel::imageCount() const
{
    return m_fileList.count();
}

QVariant ImageListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_fileList.count())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_fileList.at(index.row()).fileInfo().fileName();
    else if (role == Qt::DecorationRole)
        return m_fileList.at(index.row()).icon();
    else if (role == Qt::ToolTipRole) {
        ImageInfo info = m_fileList.at(index.row());
        return QString("<html><b>%1</b><br/><br/>Dimensions: %2<br/>Size: %3<br/>Date created: %4</html>")
                .arg(info.imagePath())
                .arg(info.dimensions())
                .arg(info.size())
                .arg(info.fileInfo().created().toString(Qt::DefaultLocaleShortDate));
    }

    return QVariant();
}

ImageInfo ImageListModel::imageInfo(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= m_fileList.count())
        return ImageInfo();

    return m_fileList.at(index.row());
}

QString ImageListModel::imagePath(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= m_fileList.count())
        return QString();

    return m_fileList.at(index.row()).imagePath();
}

void ImageListModel::addImageFileInfoList(const QFileInfoList &files)
{
    if (files.count() == 0)
        return;

    beginInsertRows(QModelIndex(), m_fileList.count(), m_fileList.count() + files.count() - 1);
    foreach (const QFileInfo &file, files)
        m_fileList.append(ImageInfo(file));
    endInsertRows();
}

void ImageListModel::clear()
{
    //m_threadPool.clear();
    beginResetModel();
    m_fileList.clear();
    m_fileCount = 0;
    endResetModel();
}

bool ImageListModel::canFetchMore(const QModelIndex &) const
{
    return m_fileCount < m_fileList.count();
}

void ImageListModel::fetchMore(const QModelIndex &)
{
    int remainder = m_fileList.count() - m_fileCount;
    int itemsToFetch = qMin(100, remainder);

    int beginIndex = m_fileCount;
    int endIndex = beginIndex + itemsToFetch;
    if (endIndex >= m_fileList.count())
        endIndex = m_fileList.count() - 1;

    beginInsertRows(QModelIndex(), beginIndex, endIndex);
    m_fileCount += itemsToFetch;
    endInsertRows();

    // start multithreaded image loading
    for (int i = beginIndex; i <= endIndex; i++) {
        ImageLoader *imageLoader = new ImageLoader(m_fileList.at(i).imagePath(), i);
        imageLoader->setScaleSize();
        connect(imageLoader, SIGNAL(imageLoaded(QImage, int, int, int)), SLOT(imageLoaded(QImage, int, int, int)));
        m_threadPool.start(imageLoader);
    }

    qDebug("ImageListModel::fetchMore(): from %d to %d", beginIndex, endIndex);
}

void ImageListModel::imageLoaded(const QImage &image, int width, int height, int index)
{
    QModelIndex modelIndex = this->index(index);
    if (!modelIndex.isValid() || index >= m_fileList.count())
        return;

    m_fileList[index].setIcon(QPixmap::fromImage(image));
    m_fileList[index].setDimensions(width, height);
    emit dataChanged(modelIndex, modelIndex);

    qDebug("ImageListModel::imageLoaded(%d)", index);
}
