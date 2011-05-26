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

#include "slideshowfilemanager.h"

#include <QMetaType>
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include <QDebug>

SlideshowFileManager::SlideshowFileManager(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<Slideshow>("Slideshow");
}

void SlideshowFileManager::loadSlideshow(const QString &file)
{
    m_path = file;
    m_singleFile = true;
    start();
}

void SlideshowFileManager::loadAllSlideshows(const QString &directory)
{
    m_path = directory;
    m_singleFile = false;
    start();
}

void SlideshowFileManager::saveSlideshow(Slideshow &slideshow, const QString &directory)
{
    if (!slideshow.hasChanged())
        return;

    QFile file(slideshow.path(directory));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("SlideshowFileManager::saveSlideshow(Slideshow, QString): cannot open file \"%s\" (%s).",
                 qPrintable(file.fileName()), qPrintable(file.errorString()));
        return;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument("1.0", true);
    xml.writeStartElement("slideshow");
    Qsm::SortFlags sort = slideshow.sortFlags();
    if (sort.testFlag(Qsm::Name) || sort.testFlag(Qsm::Date)) {
        xml.writeAttribute("sort", sort.testFlag(Qsm::Name) ? "name" : "date");
        if (sort.testFlag(Qsm::Reversed))
            xml.writeAttribute("sortOrdner", "desc");
    }
    if (!slideshow.comment().isEmpty())
        xml.writeAttribute("comment", slideshow.comment());
    foreach (const SlideshowImage &image, slideshow.images()) {
        xml.writeStartElement("image");
        xml.writeAttribute("path", QDir::toNativeSeparators(image.path()));
        if (image.randomFactor() != 1)
            xml.writeAttribute("randomFactor", QString::number(image.randomFactor()));
        if (!image.comment().isEmpty())
            xml.writeAttribute("comment", image.comment());
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();

    slideshow.resetChanged();
}

void SlideshowFileManager::saveAllSlideshows(QList<Slideshow> &slideshowList, const QString &directory)
{
    QList<Slideshow>::iterator slideshow;
    for (slideshow = slideshowList.begin(); slideshow != slideshowList.end(); ++slideshow)
        saveSlideshow(*slideshow, directory);
}

void SlideshowFileManager::run()
{
    if (m_singleFile) // single slideshow file
        loadSlideshow(QFileInfo(m_path));
    else { // slideshow directory
        QDir dir(m_path, '*' + Slideshow::FILE_EXTENSION, QDir::Name | QDir::IgnoreCase | QDir::LocaleAware,
                 QDir::Files | QDir::Readable);
        if (!dir.exists())
            return;
        foreach (const QFileInfo &fileInfo, dir.entryInfoList())
            loadSlideshow(fileInfo);
    }
}

void SlideshowFileManager::loadSlideshow(const QFileInfo &fileInfo)
{
    QFile file(fileInfo.filePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("SlideshowFileManager::loadSlideshow(QFile): cannot open file \"%s\" (%s).",
                 qPrintable(file.fileName()), qPrintable(file.errorString()));
        return;
    }

    QString slideshowName = fileInfo.fileName();
    slideshowName = slideshowName.left(slideshowName.length() - Slideshow::FILE_EXTENSION.length());
    Slideshow slideshow(slideshowName);

    QXmlStreamReader xml(&file);
    while (!xml.atEnd()) {
        if (xml.readNext() == QXmlStreamReader::StartElement) {
            const QString elementName = xml.name().toString();
            const QXmlStreamAttributes attr = xml.attributes();
            if (elementName == "slideshow") {
                if (attr.hasAttribute("sort")) {
                    Qsm::SortFlags sortFlags = Qsm::Unsorted;
                    QString sort = attr.value("sort").toString();
                    if (sort == "name")
                        sortFlags = Qsm::Name;
                    else if (sort == "date")
                        sortFlags = Qsm::Date;
                    if (attr.hasAttribute("sortOrdner")) {
                        QString sortOrdner = attr.value("sortOrdner").toString();
                        if (sortOrdner == "desc")
                            sortFlags |= Qsm::Reversed;
                    }
                    slideshow.setSortFlags(sortFlags);
                }
                if (attr.hasAttribute("comment")) {
                    QString comment = attr.value("comment").toString();
                    slideshow.setComment(comment);
                }
            } else if (elementName == "image") {
                if (attr.hasAttribute("path")) { // mandatory attribute
                    QString path = attr.value("path").toString();
                    SlideshowImage image(path);
                    if (attr.hasAttribute("randomFactor")) {
                        QString randomFactor = attr.value("randomFactor").toString();
                        image.setRandomFactor(randomFactor.toInt());
                    }
                    if (attr.hasAttribute("comment")) {
                        QString comment = attr.value("comment").toString();
                        image.setComment(comment);
                    }
                    slideshow.addImage(image);
                }
            }
        }
    }
    slideshow.resetChanged();
    if (xml.hasError()) {
        qWarning("SlideshowFileManager::loadSlideshow(QFile): parse error in file \"%s\" (%s at line %li and column %li).",
                 qPrintable(file.fileName()), qPrintable(xml.errorString()), (long)xml.lineNumber(), (long)xml.columnNumber());
    } else
        emit slideshowLoaded(slideshow);
    file.close();
}
