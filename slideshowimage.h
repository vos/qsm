#ifndef SLIDESHOWIMAGE_H
#define SLIDESHOWIMAGE_H

#include <QString>

class SlideshowImage
{
public:
    SlideshowImage(const QString &path, int loadFactor = 1, const QString &comment = QString());
    SlideshowImage(const SlideshowImage &image);

    SlideshowImage& operator =(const SlideshowImage &image);
    bool operator ==(const SlideshowImage &image);

    inline const QString& path() const { return m_path; }
    inline int loadFactor() const { return m_loadFactor; }
    inline const QString& comment() const { return m_comment; }

    inline void setLoadFactor(int loadFactor = 1) { m_loadFactor = loadFactor; }
    inline void setComment(const QString &comment = QString()) { m_comment = comment; }

private:
    QString m_path;
    int m_loadFactor;
    QString m_comment;
};

#endif // SLIDESHOWIMAGE_H
