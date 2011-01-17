#include "imagelistmodel.h"

#include <QtConcurrentMap>
#include <QFutureWatcher>
#include <QPixmap>
#include <QPainter>

QIcon ImageListModel::DEFAULT_ICON;

ImageListModel::ImageListModel(QObject *parent) :
    QAbstractListModel(parent), m_fileCount(0)
{
    m_thumbnailWatcher = new QFutureWatcher<QImage>(this);
    connect(m_thumbnailWatcher, SIGNAL(resultReadyAt(int)), SLOT(thumbnailWatcher_resultReadyAt(int)));
    connect(m_thumbnailWatcher, SIGNAL(finished()), SLOT(thumbnailWatcher_finished()));

    if (ImageListModel::DEFAULT_ICON.isNull()) {
        QPixmap icon(64, 64);
        QPainter painter(&icon);
        painter.setPen(Qt::red);
        painter.fillRect(icon.rect(), Qt::white);
        painter.drawRect(icon.rect());
        painter.drawLine(0,0, 64,64);
        painter.drawLine(64,0, 0,64);
        ImageListModel::DEFAULT_ICON = QIcon(icon);
    }
}

ImageListModel::~ImageListModel()
{
    delete m_thumbnailWatcher;
}

int ImageListModel::rowCount(const QModelIndex &) const
{
    return m_fileCount;
}

QVariant ImageListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_fileList.count())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_fileList.at(index.row()).fileName();
    else if (role == Qt::DecorationRole)
        return m_iconList.at(index.row());
    else if (role == Qt::ToolTipRole)
        return m_fileList.at(index.row()).filePath();

    return QVariant();
}

QString ImageListModel::filePath(const QModelIndex &index) const
{
    if (!index.isValid())
        return QString();

    return m_fileList.at(index.row()).filePath();
}

void ImageListModel::addImageFileInfoList(const QFileInfoList &files)
{
    if (files.size() == 0)
        return;

    beginInsertRows(QModelIndex(), m_fileList.size(), m_fileList.size() + files.size());
    m_fileList.append(files);
    for (int i = 0; i < files.count(); ++i)
        m_iconList.append(ImageListModel::DEFAULT_ICON);
    endInsertRows();
}

void ImageListModel::clear()
{
    if (m_thumbnailWatcher->isRunning()) {
        m_thumbnailWatcher->cancel();
        m_thumbnailWatcher->waitForFinished();
    }

    beginResetModel();
    m_fileList.clear();
    m_iconList.clear();
    m_fileCount = 0;
    endResetModel();
}

bool ImageListModel::canFetchMore(const QModelIndex &) const
{
    return m_fileCount < m_fileList.size();
}

void ImageListModel::fetchMore(const QModelIndex &)
{
    int remainder = m_fileList.size() - m_fileCount;
    int itemsToFetch = qMin(100, remainder);

    qDebug("fetch more: from %d to %d", m_fileCount, m_fileCount + itemsToFetch - 1);

    if (m_thumbnailWatcher->isRunning()) {
        //m_thumbnailWatcher->cancel();
        m_thumbnailWatcher->waitForFinished(); // can block the gui thread
    }

    m_thumbnailIndex = m_fileCount;

    QFileInfoList::const_iterator begin = m_fileList.begin() + m_thumbnailIndex;
    QFileInfoList::const_iterator end = begin + itemsToFetch;
    if (end > m_fileList.end())
        end = m_fileList.end();

    // start multithreaded image loading
    m_thumbnailWatcher->setFuture(QtConcurrent::mapped(begin, end, ImageListModel::createThumbnail));

    beginInsertRows(QModelIndex(), m_fileCount, m_fileCount + itemsToFetch - 1);
    m_fileCount += itemsToFetch;
    endInsertRows();
}

QImage ImageListModel::createThumbnail(const QFileInfo &fileInfo)
{
    return QImage(fileInfo.filePath()).scaled(64, 64, Qt::KeepAspectRatio, Qt::FastTransformation);
}

void ImageListModel::thumbnailWatcher_resultReadyAt(int index)
{
    qDebug("thumbnailWatcher_resultReadyAt(%d)", index);

    QImage image = m_thumbnailWatcher->resultAt(index);
    index += m_thumbnailIndex;
    m_iconList[index] = QIcon(QPixmap::fromImage(image));

    QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex);
}

void ImageListModel::thumbnailWatcher_finished()
{
    qDebug("thumbnailWatcher_finished");
    emit iconsLoaded();
}
