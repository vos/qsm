#ifndef SLIDESHOWFILEMANAGER_H
#define SLIDESHOWFILEMANAGER_H

#include <QThread>

#include "slideshow.h"

class SlideshowFileManager : public QThread
{
    Q_OBJECT

public:
    explicit SlideshowFileManager(QObject *parent = 0);

    void loadSlideshow(const QString &file);
    void loadAllSlideshows(const QString &directory);
    void saveSlideshow(const Slideshow &slideshow, const QString &directory);
    void saveAllSlideshows(const QList<Slideshow> &slideshowList, const QString &directory);

    inline bool isLoading() const { return isRunning(); }

signals:
    void slideshowLoaded(const Slideshow &slideshow);

private:
    QString m_path;
    bool m_singleFile;

    void run();
    void loadSlideshow(const QFileInfo &file);
};

#endif // SLIDESHOWFILEMANAGER_H
