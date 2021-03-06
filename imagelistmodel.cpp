/****************************************************************************
**
** Copyright (C) 2011 Alexander Vos <alexander.vos@stud.fh-swf.de>,
**                    Till Althaus <till.althaus@stud.fh-swf.de>
**
** This file is part of Qt Slideshow Manager (QSM).
**
** QSM is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** QSM is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with QSM.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "imagelistmodel.h"

#include "imageloader.h"

#include <QFileInfoList>
#include <QPixmap>
#include <QDateTime>
#include <QDir>

#include "mainwindow.h"

const QBrush ImageListModel::ERROR_BRUSH = QColor(255, 153, 153);

ImageListModel::ImageListModel(QObject *parent) :
    QAbstractListModel(parent), m_imageInfoCount(0)
{
}

Qt::ItemFlags ImageListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QVariant ImageListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_imageInfoList.count())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return m_imageInfoList.at(index.row()).fileInfo().fileName();
    case Qt::DecorationRole:
        return m_imageInfoList.at(index.row()).icon();
    case Qt::BackgroundRole:
        return (m_imageInfoList.at(index.row()).exists() ? Qt::white : ImageListModel::ERROR_BRUSH);
    case Qt::ToolTipRole:
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

bool ImageListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_imageInfoList.count())
        return false;

    if (role == Qt::EditRole) {
        QFileInfo &fileInfo = m_imageInfoList[index.row()].fileInfo();
        QString oldName = fileInfo.fileName();
        QString newName = value.toString();
        if (newName != oldName) {
            // rename the actual file
            QString oldPath = fileInfo.absoluteFilePath();
            QString newPath = fileInfo.dir().filePath(newName);
            if (QFile::rename(oldPath, newPath)) {
                // update the file info reference
                fileInfo.setFile(newPath);
                emit imageRenamed(index, newPath);
                return true;
            }
            qWarning("File \"%s\" could not be renamed to \"%s\"", qPrintable(oldPath), qPrintable(newPath));
        }
    }
    return false;
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

void ImageListModel::addImage(const ImageInfo &imageInfo, int index)
{
    if (index < 0 || index > m_imageInfoList.count())
        index = m_imageInfoList.count();
    beginInsertRows(QModelIndex(), index, index);
    m_imageInfoList.insert(index, imageInfo);
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

void ImageListModel::moveImage(const QModelIndex &index, int delta)
{
    if (!index.isValid() || index.row() >= m_imageInfoList.count() || !delta)
        return;

    int from = index.row();
    int to = qBound(0, from + delta, m_imageInfoList.count() - 1);
    if (from == to) return;
    beginMoveRows(QModelIndex(), from, from, QModelIndex(), to);
    m_imageInfoList.move(from, to);
    endMoveRows();

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
            imageLoader->setScaleSize(MainWindow::MAX_THUMBNAIL_SIZE, MainWindow::MAX_THUMBNAIL_SIZE);
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

void ImageListModel::preloadAllImages()
{
    QModelIndex parentDummy;
    while (canFetchMore(parentDummy))
        fetchMore(parentDummy);
}
