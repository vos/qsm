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
