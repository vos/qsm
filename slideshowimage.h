#ifndef SLIDESHOWIMAGE_H
#define SLIDESHOWIMAGE_H

#include <QString>

class SlideshowImage
{
public:
    SlideshowImage(const QString &path, int randomFactor = 1, const QString &comment = QString());
    SlideshowImage(const SlideshowImage &image);

    SlideshowImage& operator =(const SlideshowImage &image);
    bool operator ==(const SlideshowImage &image);

    inline const QString& path() const { return m_path; }
    inline const QString& name() const { return m_name; }
    inline int randomFactor() const { return m_randomFactor; }
    inline const QString& comment() const { return m_comment; }

    inline void setRandomFactor(int randomFactor = 1) { m_randomFactor = randomFactor; }
    inline void setComment(const QString &comment = QString()) { m_comment = comment; }

private:
    QString m_path;
    QString m_name;
    int m_randomFactor;
    QString m_comment;
};

#endif // SLIDESHOWIMAGE_H
