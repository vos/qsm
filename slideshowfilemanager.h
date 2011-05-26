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

class SlideshowFileManager : public QThread
{
    Q_OBJECT

public:
    explicit SlideshowFileManager(QObject *parent = 0);

    void loadSlideshow(const QString &file);
    void loadAllSlideshows(const QString &directory);
    void saveSlideshow(Slideshow &slideshow, const QString &directory);
    void saveAllSlideshows(QList<Slideshow> &slideshowList, const QString &directory);

    inline bool isLoading() const { return isRunning(); }

signals:
    void slideshowLoaded(const Slideshow &slideshow);

private:
    QString m_path;
    bool m_singleFile;

    void run();
    void loadSlideshow(const QFileInfo &file);
};

#endif // SLIDESHOWFILEMANAGER_H
