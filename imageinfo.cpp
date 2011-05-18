#include "imageinfo.h"

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
        return QString("%1 MB").arg(bytes / 1048576.0, 0, 'f', 2);
    else if (bytes >= 1024)
        return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 2);
    else
        return QString("%1 bytes").arg(bytes);
}

void ImageInfo::setDimensions(int width, int height)
{
    m_width = width;
    m_height = height;
}
