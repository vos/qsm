#ifndef SLIDESHOWIMAGE_H
#define SLIDESHOWIMAGE_H

#include <QString>
#include <QFileInfo>

class SlideshowImage
{
public:
    SlideshowImage(const QString &path, int randomFactor = 1, const QString &comment = QString());
    SlideshowImage(const SlideshowImage &image);

    SlideshowImage& operator =(const SlideshowImage &image);
    bool operator ==(const SlideshowImage &image);

    inline const QFileInfo& fileInfo() const { return m_fileInfo; }
    inline QString path() const { return m_fileInfo.absoluteFilePath(); }
    inline QString name() const { return m_fileInfo.fileName(); }
    inline int randomFactor() const { return m_randomFactor; }
    inline const QString& comment() const { return m_comment; }

    inline void setRandomFactor(int randomFactor = 1) { m_randomFactor = randomFactor; }
    inline void setComment(const QString &comment = QString()) { m_comment = comment; }

private:
    QFileInfo m_fileInfo;
    int m_randomFactor;
    QString m_comment;
};

#endif // SLIDESHOWIMAGE_H
