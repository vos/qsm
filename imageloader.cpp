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

#include "imageloader.h"

ImageLoader::ImageLoader(const QString &imagePath, int index, QObject *parent) :
    QObject(parent), m_imagePath(imagePath), m_index(index), m_scaleWidth(0), m_scaleHeight(0)
{
}

void ImageLoader::setScaleSize(int width, int height)
{
    m_scaleWidth = width;
    m_scaleHeight = height;
}

void ImageLoader::run()
{
    QImage image(m_imagePath);
    if (m_scaleWidth > 0 && m_scaleHeight > 0)
        emit imageLoaded(image.scaled(m_scaleWidth, m_scaleHeight, Qt::KeepAspectRatio,
                                      Qt::FastTransformation), image.width(), image.height(), m_index);
    else
        emit imageLoaded(image, image.width(), image.height(), m_index);
}
