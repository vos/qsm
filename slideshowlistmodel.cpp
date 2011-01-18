#include "slideshowlistmodel.h"

SlideshowListModel::SlideshowListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int SlideshowListModel::rowCount(const QModelIndex &) const
{
    return list.count();
}

QVariant SlideshowListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= list.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::ToolTipRole || role == Qt::EditRole)
        return list.at(index.row());

    return QVariant();
}

void SlideshowListModel::addSlideshow(const QString &slideshow)
{
    beginInsertRows(QModelIndex(), list.count(), list.count());
    list.append(slideshow);
    endInsertRows();
}

void SlideshowListModel::clear()
{
    list.clear();
    reset();
}
