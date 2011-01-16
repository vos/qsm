#include "imagelistmodel.h"

#include <QPixmap>

ImageListModel::ImageListModel(QObject *parent) :
    QAbstractListModel(parent), m_fileCount(0)
{
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

void ImageListModel::addImageFileInfoList(const QFileInfoList &files)
{
    if (files.size() == 0)
        return;

    beginInsertRows(QModelIndex(), m_fileList.size(), m_fileList.size() + files.size());
    m_fileList.append(files);
    foreach (const QFileInfo &fileInfo, files)
        m_iconList.append(QIcon(QPixmap(fileInfo.filePath()).scaled(64, 64, Qt::KeepAspectRatio, Qt::FastTransformation)));
    endInsertRows();
}

void ImageListModel::clear()
{
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

void ImageListModel::fetchMore(const QModelIndex &parent)
{
    int remainder = m_fileList.size() - m_fileCount;
    int itemsToFetch = qMin(100, remainder);

    beginInsertRows(parent, m_fileCount, m_fileCount + itemsToFetch - 1);
    m_fileCount += itemsToFetch;
    endInsertRows();
}
