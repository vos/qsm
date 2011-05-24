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
    inline int rowCount(const QModelIndex & = QModelIndex()) const { return m_slideshowList.count(); }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Slideshow* slideshow(const QModelIndex &index) const;
    Slideshow* slideshow(const QString &name) const;

    inline QList<Slideshow>& slideshowList() { return m_slideshowList; }
    inline Slideshow* currentSlideshow() const { return m_currentSlideshow; }
    inline const QModelIndex& currentSlideshowIndex() const { return m_currentSlideshowIndex; }

signals:
    void slideshowRenamed(const QString &oldName, const QString &newName);

public slots:
    QModelIndex addSlideshow(const Slideshow &slideshow);
    bool removeSlideshow(const QModelIndex &index);
    void clear();
    int addImage(const SlideshowImage &image);
    void removeImage(const QModelIndex &index);
    bool setCurrentSlideshow(const QModelIndex &index);

private:
    QList<Slideshow> m_slideshowList;
    Slideshow *m_currentSlideshow;
    QModelIndex m_currentSlideshowIndex;
};

#endif // SLIDESHOWLISTMODEL_H
