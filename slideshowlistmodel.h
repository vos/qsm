#ifndef SLIDESHOWLISTMODEL_H
#define SLIDESHOWLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class SlideshowListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SlideshowListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public slots:
    void addSlideshow(const QString &slideshow);
    void clear();

private:
    QStringList list;

};

#endif // SLIDESHOWLISTMODEL_H
