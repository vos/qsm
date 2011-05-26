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

#include "imageinfo.h"

#include <QCoreApplication>
#include <QPixmap>
#include <QPainter>

#include "mainwindow.h"

QIcon ImageInfo::DEFAULT_ICON;

ImageInfo::ImageInfo(const QFileInfo &fileInfo)
{
    if (ImageInfo::DEFAULT_ICON.isNull()) {
        QPixmap icon(MainWindow::MAX_THUMBNAIL_SIZE, MainWindow::MAX_THUMBNAIL_SIZE);
        QPainter painter(&icon);
        painter.setBrush(Qt::white);
        painter.setPen(Qt::red);
        painter.drawRect(0,0, icon.rect().width()-1, icon.rect().height()-1);
        painter.drawLine(0,0, MainWindow::MAX_THUMBNAIL_SIZE,MainWindow::MAX_THUMBNAIL_SIZE);
        painter.drawLine(MainWindow::MAX_THUMBNAIL_SIZE,0, 0,MainWindow::MAX_THUMBNAIL_SIZE);
        ImageInfo::DEFAULT_ICON = QIcon(icon);
    }

    m_fileInfo = fileInfo;
    m_icon = ImageInfo::DEFAULT_ICON;
    m_width = m_height = 0;
}

ImageInfo::ImageInfo(const ImageInfo &obj)
{
    m_fileInfo = obj.m_fileInfo;
    m_icon = obj.m_icon;
    m_width = obj.m_width;
    m_height = obj.m_height;
}

ImageInfo& ImageInfo::operator =(const ImageInfo &obj)
{
    if (this != &obj) {
        m_fileInfo = obj.m_fileInfo;
        m_icon = obj.m_icon;
        m_width = obj.m_width;
        m_height = obj.m_height;
    }
    return *this;
}

QString ImageInfo::dimensions() const
{
    return QString("%1 x %2").arg(m_width).arg(m_height);
}

QString ImageInfo::size() const
{
    qint64 bytes = m_fileInfo.size();
    if (bytes >= 1048576)
        return QString(QCoreApplication::translate("ImageInfo", "%1 MB")).arg(bytes / 1048576.0, 0, 'f', 2);
    else if (bytes >= 1024)
        return QString(QCoreApplication::translate("ImageInfo", "%1 KB")).arg(bytes / 1024.0, 0, 'f', 2);
    else
        return QString(QCoreApplication::translate("ImageInfo", "%1 bytes")).arg(bytes);
}

void ImageInfo::setDimensions(int width, int height)
{
    m_width = width;
    m_height = height;
}
