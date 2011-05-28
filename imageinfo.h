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

#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QFileInfo>
#include <QIcon>

/*!
  \brief The ImageInfo class provides information about the image location,
         dimension and holds a thumbnail representation of the image.
  \sa SlideshowImage, ImageListModel
 */
class ImageInfo
{

public:

    /*!
      \brief Constructs a new ImageInfo with the provided QFileInfo \a fileInfo
             and a default icon.
      \param fileInfo The QFileInfo object which should represent an image file.
     */
    ImageInfo(const QFileInfo &fileInfo = QFileInfo());

    /*!
      \brief Constructs a new ImageInfo that is a copy of the given \a imageInfo.
      \param imageInfo The ImageInfo to copy.
     */
    ImageInfo(const ImageInfo &imageInfo);

    /*!
      \brief Makes a copy of the given \a imageInfo and assigns it to this ImageInfo.
      \param imageInfo The ImageInfo to copy.
      \return A reference to this ImageInfo.
     */
    ImageInfo& operator =(const ImageInfo &imageInfo);

    /*!
      \brief Returns the QFileInfo representing the image file.
      \return The QFileInfo representing the image file.
     */
    inline const QFileInfo& fileInfo() const { return m_fileInfo; }

    /*!
      \brief Returns the QFileInfo representing the image file.
      \return The QFileInfo representing the image file.
     */
    inline QFileInfo& fileInfo() { return m_fileInfo; }

    /*!
      \brief Returns the absolute image path.
      \return The absolute image path.
      \sa fileInfo()
     */
    inline QString imagePath() const { return m_fileInfo.absoluteFilePath(); }

    /*!
      \brief Returns \c true if the image exists; otherwise returns \c false.
      \return \c True if the image exists; otherwise \c false.
      \sa fileInfo()
     */
    inline bool exists() const { return m_fileInfo.exists(); }

    /*!
      \brief Returns the thumbnail icon of this image.
      \return The thumbnail icon.
      \sa setIcon()
     */
    inline const QIcon& icon() const { return m_icon; }

    /*!
      \brief Returns the width in pixels of this image.
      \return The width in pixels.
      \sa setDimensions(), dimensions(), height()
     */
    inline int width() const { return m_width; }

    /*!
      \brief Returns the height in pixels of this image.
      \return The height in pixels.
      \sa setDimensions(), dimensions(), width()
     */
    inline int height() const { return m_height; }

    /*!
      \brief Returns the image dimensions in pixels of this image as a QString.
      \return The image dimensions in pixels.
      \sa width(), height(), setDimensions()
     */
    QString dimensions() const;

    /*!
      \brief Returns the size in bytes of this image as a QString.
      \return The size in bytes.
     */
    QString size() const;

    /*!
      \brief Sets the thumbnail icon to \a icon.
      \param icon The new thumbnail icon.
      \sa icon()
     */
    inline void setIcon(const QIcon &icon) { m_icon = icon; }

    /*!
      \brief Sets image dimensions to \a height and \a width pixels.
      \param width The new image width in pixels.
      \param height The new image height in pixels.
      \sa width(), height(), dimensions()
     */
    void setDimensions(int width, int height);

private:

    static QIcon DEFAULT_ICON; //!< The default thumbnail icon for all images.

    QFileInfo m_fileInfo;      //!< The file information.
    QIcon m_icon;              //!< The thumbnail icon.
    int m_width;               //!< The image width in pixels.
    int m_height;              //!< The image height in pixels.

};

#endif // IMAGEINFO_H
