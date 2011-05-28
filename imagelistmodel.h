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

/*!
  \brief The ImageListModel class provides a model of multiple images which can
         be presented to the user with the ImageListView class.
  \sa ImageListView
 */
class ImageListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    /*!
      \brief Constructs a new ImageListModel with the given \a parent.
      \param parent The parent widget.
     */
    explicit ImageListModel(QObject *parent = 0);

    /*!
      \brief Returns the item flags for the given \a index.
      \param index The QModelIndex of the item.
      \return The item flags.
     */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /*!
      \brief Returns the image count.
      \note The image count is less than or equal the rowCount().
      \return The image count.
      \sa rowCount()
     */
    inline int imageCount() const { return m_imageInfoList.count(); }

    /*!
      \brief Returns the row count.
      \return The row count.
      \sa imageCount()
     */
    inline int rowCount(const QModelIndex & = QModelIndex()) const { return m_imageInfoCount; }

    /*!
      \brief Returns the data stored under the given \a role for the item
             referred to by the \a index.
      \param index The QModelIndex of the item.
      \param role The role of the item.
      \return The stored data.
      \sa setData()
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /*!
      \brief Sets the \a role data for the item at \a index to \a value.
      \param index The QModelIndex of the item.
      \param value The data of the item.
      \param role The role of the item.
      \return \c True if successful; otherwise \c false.
      \sa data(), imageRenamed()
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    /*!
      \brief Returns the image information for the item referred to by the \a index.
      \param index The QModelIndex of the item.
      \return The ImageInfo.
      \sa imagePath(), data()
     */
    ImageInfo imageInfo(const QModelIndex &index) const;

    /*!
      \brief Returns the image path for the item referred to by the \a index.
      \param index The QModelIndex of the item.
      \return The path of the image.
      \sa imageInfo(), data()
     */
    QString imagePath(const QModelIndex &index) const;

    /*!
      \brief Returns \c true if all thumbnails are loaded; otherwise returns \c false.
      \return \c True if all thumbnails are loaded; otherwise \c false.
      \sa preloadAllImages()
     */
    inline bool allThumbnailsLoaded() const { return rowCount() == imageCount(); }

signals:

    /*!
      \brief This signal is emitted whenever the data of the image model changes.
     */
    void changed();

    /*!
      \brief This signal is emitted after an image has been renamed.
      \param index The index of the renamed image.
      \param newPath The new path of the image.
      \sa setData()
     */
    void imageRenamed(const QModelIndex &index, const QString &newPath);

public slots:

    /*!
      \brief Adds an image to this model.
      \param imageInfo The image information of the image.
      \param index The index where the image should be inserted or \c -1 to
                   append the image at the end.
      \sa addImages(), moveImage(), removeImage(), clear()
     */
    void addImage(const ImageInfo &imageInfo, int index = -1);

    /*!
      \brief Appends multiple images to this model.
      \param files The file information list of the images to append.
      \sa addImage(), moveImage(), removeImage(), clear()
     */
    void addImages(const QFileInfoList &files);

    /*!
      \brief Moves one image referred to by the \a index by \a delta steps.
      \param index The QModelIndex of the item which should be moved.
      \param delta The amount of steps to move the image. Can also be negative
                   to move the image up.
      \sa removeImage()
      \bug Unexpected behavior when moving by delta +1 (could crash the whole app).
     */
    void moveImage(const QModelIndex &index, int delta);

    /*!
      \brief Removes an image from this model.
      \param index The QModelIndex of the item to remove.
      \sa addImages(), moveImage(), removeImages(), clear()
     */
    void removeImage(const QModelIndex &index);

    /*!
      \brief Removes multiple images from this model.
      \param startIndex The QModelIndex of the first item to remove.
      \param endIndex The QModelIndex of the last item to remove.
      \sa addImages(), moveImage(), removeImage(), clear()
     */
    void removeImages(const QModelIndex &startIndex, const QModelIndex &endIndex);

    /*!
      \brief Removes all images from this model.
      \sa addImage(), addImages(), removeImage(), removeImages()
     */
    void clear();

    /*!
      \brief Preloads all images in this model.
     */
    void preloadAllImages();

private slots:

    /*!
      \brief Sets the loaded thumbnail at the given \a index.
      \param image The loaded image.
      \param width The width of the loaded image.
      \param height The height of the loaded image.
      \param index The model index where the loaded image should be inserted.
      \sa fetchMore()
     */
    void thumbnailLoaded(const QImage &image, int width, int height, int index);

private:

    static const QBrush ERROR_BRUSH;   //!< The default brush to indicate an error.

    QList<ImageInfo> m_imageInfoList;  //!< The list of image informations in this model.
    int m_imageInfoCount;              //!< The actual count of preloaded images.

    ImageLoaderPool m_imageLoaderPool; //!< The thread pool to load multiple images.

    /*!
      \brief Returns \c true if there are more images available to load;
             otherwise returns \c false.
      \return \c true if there are more images available to load;
              otherwise \c false.
     */
    inline bool canFetchMore(const QModelIndex &) const { return m_imageInfoCount < m_imageInfoList.count(); }

    /*!
      \brief Fetches the next bunch of availably images for the items with
             the specified \a parent.
      \param parent The index of the parent model.
     */
    void fetchMore(const QModelIndex &parent);

};

#endif // IMAGELISTMODEL_H
