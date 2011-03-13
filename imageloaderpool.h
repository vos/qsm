#ifndef IMAGELOADERPOOL_H
#define IMAGELOADERPOOL_H

#include <QThreadPool>

class ImageLoaderPool : public QThreadPool
{
    Q_OBJECT

public:
    explicit ImageLoaderPool(QObject *parent = 0);

public slots:
    void clear();
};

#endif // IMAGELOADERPOOL_H
