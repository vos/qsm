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

#ifndef SLIDESHOWFILEMANAGER_H
#define SLIDESHOWFILEMANAGER_H

#include <QThread>

#include "slideshow.h"

/*!
  \brief The SlideshowFileManager class manages the communication between the
         Slideshows in memory and the file system.

  SlideshowFileManager loads slideshows from the file system into memory and
  saves slideshows from memory back to the file system for persistent storage.

  Slideshow loading is done asynchronously with a background worker thread,
  but the save operations will happen synchronously to prevent memory corruption
  and to accelerate program shutdown times, because the slideshows usually gets
  saved to disk if the application is closed.

  \sa Slideshow
 */
class SlideshowFileManager : public QThread
{
    Q_OBJECT

public:

    /*!
      \brief Constructs a new SlideshowFileManager.
      \param parent The parent object.
     */
    explicit SlideshowFileManager(QObject *parent = 0);

    /*!
      \brief Loads a single slideshow from the file system into memory.
      \param file The path to the slideshow file to load.
      \note This method returns immediately and starts a background thread
            to load the actual file into memory. After the slideshow is loaded,
            the slideshowLoaded() signal will be emitted.
      \sa slideshowLoaded(), loadAllSlideshows(), isLoading(),
          saveSlideshow(), saveAllSlideshows()
     */
    void loadSlideshow(const QString &file);

    /*!
      \brief Loads all slideshows inside a folder from the file system into memory.
      \param directory The directory to the slideshow files to load.
      \note This method returns immediately and starts a background thread
            to load the actual files into memory. For each loaded slideshow
            the slideshowLoaded() signal will be emitted.
      \sa slideshowLoaded(), loadSlideshow(), isLoading(),
          saveSlideshow(), saveAllSlideshows()
     */
    void loadAllSlideshows(const QString &directory);

    /*!
      \brief Saves a single slideshow from memory to the file system.
      \param slideshow The slideshow to save.
      \param directory The directory where the slideshow should be saved.
      \sa loadSlideshow(), loadAllSlideshows(), saveAllSlideshows()
     */
    void saveSlideshow(Slideshow &slideshow, const QString &directory);

    /*!
      \brief Saves a list of slideshows from memory to the file system.
      \param slideshowList The list of slideshows to save.
      \param directory The directory where the slideshows should be saved.
      \sa loadSlideshow(), loadAllSlideshows(), saveSlideshow()
     */
    void saveAllSlideshows(QList<Slideshow> &slideshowList, const QString &directory);

    /*!
      \brief Returns \c true if background thread is loading slideshows;
             otherwise returns \c false.
      \return \c True if background thread is loading slideshows;
              otherwise \c false.
     */
    inline bool isLoading() const { return isRunning(); }

signals:

    /*!
      \brief This signal is emitted after a slideshow has been loaded.
      \param slideshow The loaded slideshow.
      \sa loadSlideshow(), loadAllSlideshows(), isLoading()
     */
    void slideshowLoaded(const Slideshow &slideshow);

private:

    /*!
      \brief The path to a single slideshow file or the folder where the
             slideshows are stored if multiple slideshows should be loaded.
     */
    QString m_path;

    /*!
      \brief Indicates if a single slideshow or a folder of slideshows should
             be loaded.
     */
    bool m_singleFile;

    /*!
      \brief Performs the actual slideshow loading using a separate worker thread.
      \sa start(), isRunning(), isLoading(), loadSlideshow(), loadAllSlideshows()
     */
    void run();

    /*!
      \brief Loads a single slideshow file from the file system and emits the
             \sa slideshowLoaded() signal if successful.
      \param file The slieshow file to load.
      \sa slideshowLoaded(), loadSlideshow(), loadAllSlideshows()
     */
    void loadSlideshow(const QFileInfo &file);

};

#endif // SLIDESHOWFILEMANAGER_H
