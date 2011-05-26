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

class ImageLoader : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit ImageLoader(const QString &imagePath, int index = 0, QObject *parent = 0);
    void setScaleSize(int width = 64, int height = 64);
    void run();

signals:
    void imageLoaded(const QImage &image, int width, int height, int index);

private:
    QString m_imagePath;
    int m_index;
    int m_scaleWidth;
    int m_scaleHeight;
};

#endif // IMAGELOADER_H
