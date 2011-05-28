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

#ifndef IMAGELOADERTHREAD_H
#define IMAGELOADERTHREAD_H

#include <QThread>

class ImageLoader;

/*!
  \brief The ImageLoaderThread class provides a thin wrapper around a ImageLoader.
  \sa ImageLoader, ImageLoaderPool
 */
class ImageLoaderThread : public QThread
{
    Q_OBJECT

public:

    /*!
      \brief Constructs a new ImageLoaderThread to run an ImageLoader inside
             a separate thread.
      \param loader The image loader to be encapsulated by this thread.
      \param parent The parent object.
      \note If ImageLoader::autoDelete() returns \c true this thread will take
            ownership of the image loader and deletes it automatically after
            the execution of run().
     */
    explicit ImageLoaderThread(ImageLoader *loader, QObject *parent = 0);

    /*!
      \brief Destroys the ImageLoaderThread and the associated image loader,
             if ImageLoader::autoDelete() returns \c true.
     */
    ~ImageLoaderThread();

    /*!
      \brief Returns the image loader associated with this thread.
      \return The image loader associated with this thread.
     */
    inline ImageLoader* loader() const { return m_loader; }

private:

    ImageLoader *m_loader; //!< The image loader associated with this thread.

    /*!
      \brief Runs the image loader inside a separate thread.
      \sa ImageLoader::run(), start()
     */
    void run();

};

#endif // IMAGELOADERTHREAD_H
