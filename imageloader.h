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

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QObject>
#include <QRunnable>
#include <QImage>

/*!
  \brief The ImageLoader class provides a simple image loader for use with the
         ImageLoaderPool or QThreadPool classes.
  \sa ImageLoaderPool, ImageLoaderThread
 */
class ImageLoader : public QObject, public QRunnable
{
    Q_OBJECT

public:

    /*!
      \brief Constructs a new ImageLoader with the provided \a imagePath and \a index.
      \param imagePath The file path of the image to load.
      \param index The model index where the image should be inserted.
      \param parent The parent object.
      \note Auto-deletion is enabled by default.
     */
    explicit ImageLoader(const QString &imagePath,
                         int index = 0,
                         QObject *parent = 0);

    /*!
      \brief Sets the size of the scaled image to \a width and \a height.
      \param width The width of the scaled image.
      \param height The height of the scaled image.
      \note If either \a width or \a height is less than or equal \c 0 the image
            will not be scaled and remains at its original size.
     */
    void setScaleSize(int width = 64, int height = 64);

    /*!
      \brief This method performs the actual image loading. It should be called
             inside a separate thread.
      \sa ImageLoaderThread::run()
      */
    void run();

signals:

    /*!
      \brief This signal is emitted after the image has been loaded.
      \param image The loaded image.
      \param width The width of the loaded image.
      \param height The height of the loaded image.
      \param index The model index where the loaded image should be inserted.
     */
    void imageLoaded(const QImage &image, int width, int height, int index);

private:

    QString m_imagePath; //!< The file path of the image to load.
    int m_index;         //!< The model index where the image should be inserted.
    int m_scaleWidth;    //!< The width of the scaled image.
    int m_scaleHeight;   //!< The height of the scaled image.

};

#endif // IMAGELOADER_H
