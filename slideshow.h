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

#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include <QString>
#include <QList>

#include "qsm.h"
#include "slideshowimage.h"

/*!
  \brief The Slideshow class defines a single slideshow which contains a list
         of SlideshowImages.
  \sa SlideshowImage, SlideshowListModel
 */
class Slideshow
{

public:

    static const QString FILE_EXTENSION; //!< The file extension of all slideshow files.

    /*!
      \brief Constructs a new Slideshow with the provided \a name.
      \param name The name of the slideshow.
      \param sortOrder The sort ordner of the slideshow images.
      \param comment The comment for the slideshow.
      \sa setName(), setSortOrder(), setComment()
     */
    Slideshow(const QString &name = QString(),
              Qsm::SortFlags sortOrder = Qsm::Unsorted,
              const QString &comment = QString());

    /*!
      \brief Constructs a new Slideshow that is a copy of the given \a slideshow.
      \param slideshow The Slideshow to copy.
     */
    Slideshow(const Slideshow &slideshow);

    /*!
      \brief Makes a copy of the given \a slideshow and assigns it to this Slideshow.
      \param slideshow The Slideshow to copy.
      \return A reference to this Slideshow.
     */
    Slideshow& operator =(const Slideshow &slideshow);

    /*!
      \brief Returns the name of this slideshow.
      \return The name of this slideshow.
      \sa setName()
     */
    inline const QString& name() const { return m_name; }

    /*!
      \brief Sets the name of this slideshow.
      \param name The name for this slideshow.
      \sa name()
     */
    void setName(const QString &name);

    /*!
      \brief Returns the path on the file system.
      \param dir The directory where the slideshow should be located.
      \return The path on the file system.
      \sa name()
     */
    QString path(const QString &dir) const;

    /*!
      \brief Returns the sort order of the images inside this slideshow.
      \return The sort order of the images.
      \sa setSortOrder(), sort()
     */
    inline Qsm::SortFlags sortOrder() const { return m_sort; }

    /*!
      \brief Sets the sort order of the images inside this slideshow.
      \param sortOrder The sort order for the images.
      \return \c True if the sort order has changed; otherwise \c false.
      \note The images will be reordered if the sort order changes.
      \sa sortOrder(), sort()
     */
    bool setSortOrder(Qsm::SortFlags sortOrder = Qsm::Unsorted);

    /*!
      \brief Returns the comment of this slideshow.
      \return The comment of this slideshow.
      \sa setComment()
     */
    inline const QString& comment() const { return m_comment; }

    /*!
      \brief Sets the comment of this slideshow.
      \param comment The comment for this slideshow.
      \sa comment()
     */
    void setComment(const QString &comment = QString());

    /*!
      \brief Adds an image to this slideshow according to the internal sort order.
      \param image The image to add to this slideshow.
      \sa image(), images(), moveImage(), removeImage(), imageCount(),
          clearImages(), sortOrder(), setSortOrder(), sort()
     */
    int addImage(const SlideshowImage &image);

    /*!
      \brief Returns the image at position \a index or \c NULL if the \a index is invalid.
      \param index The index of the image in this slideshow.
      \return The image or \c NULL if the index is invalid.
      \sa images(), moveImage(), removeImage(), imageCount(), clearImages()
     */
    SlideshowImage* image(int index);

    /*!
      \brief Returns a reference to the internal image list.
      \return A reference to the internal image list.
      \sa image(), moveImage(), removeImage(), imageCount(), clearImages(), sort()
     */
    inline QList<SlideshowImage>& images() { return m_images; }

    /*!
      \brief Moves one image by \a delta positions.
      \param index The index of the image to move.
      \param delta The amount of steps to move the image. Can also be negative
                   to move the image up.
      \return The new index of the moved image or \c -1 if the move operation
              was not possible (invalid \a index or \a delta).
      \note The sort order will be set to \c Qsm::Unsorted if the move operation
            is successful.
      \sa addImage(), image(), images(), removeImage(), sortOrder(),
          setSortOrder(), sort()
     */
    int moveImage(int index, int delta);

    /*!
      \brief Removes the image at \a index from the slideshow.
      \param index The index of the image to remove.
      \return \c True if the image was removed successfully, otherwise \c false.
      \sa addImage(), image(), images(), moveImage(), imageCount(), clearImages()
     */
    bool removeImage(int index);

    /*!
     \brief Returns the number of images in this slideshow.
     \return The number of images.
     */
    inline int imageCount() const { return m_images.count(); }

    /*!
     \brief Removes all images from this slideshow.
     \sa removeImage(), image(), images(), imageCount()
     */
    void clearImages();

    /*!
      \brief Sorts all images in this slideshow according to the sort order.
      \sa sortOrder(), setSortOrder()
     */
    void sort();

    /*!
      \brief Returns \c true if there are corrupted images in this slideshow;
             otherwise returns \c false.
      \return \c True if there are corrupted images; otherwise \c false.
      \note Corrupted images are usually images which cannot be loced on the disk
            or cannot be read from the file system. The path can also be point to
            a non image file and will therefore be marked as invalid.
       \sa removeAllCorruptedImages(), removeImage(), image(), images()
     */
    bool hasCorruptedImages() const;

    /*!
      \brief Removes all corrupted images from this slideshow.
      \sa hasCorruptedImages(), removeImage(), images(), clearImages();
     */
    void removeAllCorruptedImages();

    /*!
      \brief Returns the change state of this slideshow, which indicates that
             the internal data structure has changed since the last save.
      \return \c True if the internal state has changed; otherwise \c false.
      \sa setChanged(), resetChanged()
     */
    inline bool hasChanged() const { return m_changed; }

    /*!
      \brief Sets the state of this slideshow to \b changed.
      \note The state gets automatically set to \b changed if you operate
            directly with the methods in this class. The manual override is only
            useful if the state gets changed through a reference to the slideshow
            images in this slideshow.
      \sa hasChanged(), resetChanged()
     */
    inline void setChanged() { m_changed = true; }

    /*!
      \brief Resets the changed state.
      \note The change state should only be reset if the slideshow has been
            successfully saved to the file system.
      \sa hasChanged(), setChanged()
     */
    inline void resetChanged() { m_changed = false; }

private:

    /*!
      \brief Helper method to sort the slideshow images by name in ascending order.
      \param a The first slideshow image.
      \param b The second slideshow image.
      \return \c True if the first image will be ordered before the second image;
              otherwise \c false.
     */
    static bool sortNameAsc(const SlideshowImage &a, const SlideshowImage &b);

    /*!
      \brief Helper method to sort the slideshow images by name in descending order.
      \param a The first slideshow image.
      \param b The second slideshow image.
      \return \c True if the first image will be ordered before the second image;
              otherwise \c false.
     */
    static bool sortNameDesc(const SlideshowImage &a, const SlideshowImage &b);

    /*!
      \brief Helper method to sort the slideshow images by date in ascending order.
      \param a The first slideshow image.
      \param b The second slideshow image.
      \return \c True if the first image will be ordered before the second image;
              otherwise \c false.
     */
    static bool sortDateAsc(const SlideshowImage &a, const SlideshowImage &b);

    /*!
      \brief Helper method to sort the slideshow images by date in descending order.
      \param a The first slideshow image.
      \param b The second slideshow image.
      \return \c True if the first image will be ordered before the second image;
              otherwise \c false.
     */
    static bool sortDateDesc(const SlideshowImage &a, const SlideshowImage &b);

    QString m_name;                 //!< The name of this slideshow.
    Qsm::SortFlags m_sort;          //!< The sort order of this slideshow.
    QString m_comment;              //!< The comment of this slideshow.
    QList<SlideshowImage> m_images; //!< The list of images.
    bool m_changed;                 //!< Indicates that the slideshow has changed.

    /*!
      \brief The function pointer to the sort method.
      \param a The first slideshow image.
      \param b The second slideshow image.
      \return \c True if the first image will be ordered before the second image;
              otherwise \c false.
      \note The pointer can be \c NULL if the sort order is set to \c Qsm::Unsorted.
      \sa sortNameAsc(), sortNameDesc(), sortDateAsc(), sortDateDesc(), sort()
     */
    bool (*m_comparator)(const SlideshowImage &a, const SlideshowImage &b);

};

#endif // SLIDESHOW_H
