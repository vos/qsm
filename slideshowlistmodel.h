#ifndef SLIDESHOWLISTMODEL_H
#define SLIDESHOWLISTMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "slideshow.h"

class SlideshowListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SlideshowListModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Slideshow* slideshow(const QModelIndex &index) const;

    inline const QList<Slideshow>& slideshowList() const { return m_slideshowList; }
    inline Slideshow* currentSlideshow() const { return m_currentSlideshow; }
    inline const QModelIndex& currentSlideshowIndex() const { return m_currentSlideshowIndex; }

public slots:
    QModelIndex addSlideshow(const Slideshow &slideshow);
    bool removeSlideshow(const QModelIndex &index);
    void clear();
    void addImage(const SlideshowImage &image);
    void removeImage(const QModelIndex &index);
    bool setCurrentSlideshow(const QModelIndex &index);

private:
    QList<Slideshow> m_slideshowList;
    Slideshow *m_currentSlideshow;
    QModelIndex m_currentSlideshowIndex;
};

#endif // SLIDESHOWLISTMODEL_H
