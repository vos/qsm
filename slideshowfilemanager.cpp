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

void SlideshowFileManager::saveSlideshow(const Slideshow &slideshow, const QString &directory)
{
    qDebug() << slideshow.path(directory) << slideshow.hasChanged();
}

void SlideshowFileManager::saveAllSlideshows(const QList<Slideshow> &slideshowList, const QString &directory)
{
    foreach (const Slideshow &slideshow, slideshowList)
        saveSlideshow(slideshow, directory);
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
                    slideshow.setSort(sortFlags);
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
