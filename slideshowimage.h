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

#ifndef SLIDESHOWIMAGE_H
#define SLIDESHOWIMAGE_H

#include <QString>
#include <QFileInfo>

/*!
  \brief The SlideshowImage class defines a single image from a Slideshow.
  \sa Slideshow
 */
class SlideshowImage
{

public:

    /*!
      \brief Constructs a new SlideshowImage from the provided image \a path.
      \param path The path of the image file.
      \param randomFactor The random factor is used for the weighted random
             algorithm if a slideshow gets played in random order.
      \param comment The comment for the image.
      \sa setPath(), setRandomFactor(), setComment()
     */
    SlideshowImage(const QString &path,
                   int randomFactor = 1,
                   const QString &comment = QString());

    /*!
      \brief Constructs a new SlideshowImage that is a copy of the given \a image.
      \param image The SlideshowImage to copy.
     */
    SlideshowImage(const SlideshowImage &image);

    /*!
      \brief Makes a copy of the given \c image and assigns it to this SlideshowImage.
      \param image The SlideshowImage to copy.
      \return A reference to this SlideshowImage.
     */
    SlideshowImage& operator =(const SlideshowImage &image);

    /*!
      \brief Returns \c true if the \a image is equal to this image;
             otherwise returns \c false.
      \param image The other image to compare to.
      \return \c True if the \a image is equal to this image; otherwise \c false.
     */
    bool operator ==(const SlideshowImage &image);

    /*!
      \brief Returns the file information of this image.
      \return The file information of this image.
      \sa path(), name()
     */
    inline const QFileInfo& fileInfo() const { return m_fileInfo; }

    /*!
      \brief Returns the absolute file path of this image.
      \return The file absolute path of this image.
      \sa setPath(), fileInfo(), name()
     */
    inline QString path() const { return m_fileInfo.absoluteFilePath(); }

    /*!
      \brief Returns the file name of this image.
      \return The file name of this image.
      \sa setName(), path(), fileInfo()
     */
    inline QString name() const { return m_fileInfo.fileName(); }

    /*!
      \brief Returns the random factor of this image.
      \return The random factor of this image.
      \note The random factor is used for the weighted random algorithm if
            a slideshow gets played in random order.
      \sa setRandomFactor()
     */
    inline int randomFactor() const { return m_randomFactor; }

    /*!
      \brief Returns the comment of this image.
      \return The comment of this image.
      \sa setComment()
     */
    inline const QString& comment() const { return m_comment; }

    /*!
      \brief Sets the file path of this image.
      \param path The file path for this image.
      \sa path(), fileInfo()
     */
    inline void setPath(const QString &path) { m_fileInfo.setFile(path); }

    /*!
      \brief Sets the random factor of this image.
      \param randomFactor The random factor for this image.
      \sa randomFactor()
     */
    inline void setRandomFactor(int randomFactor = 1) { m_randomFactor = randomFactor; }

    /*!
      \brief Sets the comment of this image.
      \param comment The comment for this image.
      \sa comment()
     */
    inline void setComment(const QString &comment = QString()) { m_comment = comment; }

private:

    QFileInfo m_fileInfo; //!< The file information of this image.
    int m_randomFactor;   //!< The random factor of this image.
    QString m_comment;    //!< The comment of this image.

};

#endif // SLIDESHOWIMAGE_H
