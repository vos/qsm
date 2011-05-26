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

    Qt::ItemFlags flags(const QModelIndex &index) const;
    inline int imageCount() const { return m_imageInfoList.count(); }
    inline int rowCount(const QModelIndex & = QModelIndex()) const { return m_imageInfoCount; }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    ImageInfo imageInfo(const QModelIndex &index) const;
    QString imagePath(const QModelIndex &index) const;

    inline bool allThumbnailsLoaded() const { return rowCount() == imageCount(); }

signals:
    void changed();
    void imageRenamed(const QModelIndex &index, const QString &newPath);

public slots:
    void addImage(const ImageInfo &imageInfo, int index = -1);
    void addImages(const QFileInfoList &files);
    void moveImage(const QModelIndex &index, int delta);
    void removeImage(const QModelIndex &index);
    void removeImages(const QModelIndex &startIndex, const QModelIndex &endIndex);
    void clear();
    void preloadAllImages();

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
