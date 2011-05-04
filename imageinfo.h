#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QFileInfo>
#include <QIcon>

class ImageInfo
{
public:
    ImageInfo(const QFileInfo &fileInfo = QFileInfo());
    ImageInfo(const ImageInfo &obj);

    ImageInfo& operator =(const ImageInfo &obj);

    inline const QFileInfo fileInfo() const { return m_fileInfo; }
    inline QString imagePath() const { return m_fileInfo.absoluteFilePath(); }
    inline bool exists() const { return m_fileInfo.exists(); }
    inline const QIcon icon() const { return m_icon; }
    inline int width() const { return m_width; }
    inline int height() const { return m_height; }
    QString dimensions() const;
    QString size() const;

    inline void setIcon(const QIcon &icon) { m_icon = icon; }
    void setDimensions(int width, int height);

private:
    static QIcon DEFAULT_ICON;

    QFileInfo m_fileInfo;
    QIcon m_icon;
    int m_width;
    int m_height;
};

#endif // IMAGEINFO_H
